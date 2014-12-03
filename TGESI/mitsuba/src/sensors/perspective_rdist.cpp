/*
    This file is part of Mitsuba, a physically based rendering system.

    Copyright (c) 2007-2014 by Wenzel Jakob and others.

    Mitsuba is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Mitsuba is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <mitsuba/render/sensor.h>
#include <mitsuba/render/medium.h>
#include <mitsuba/core/track.h>
#include <mitsuba/core/frame.h>

MTS_NAMESPACE_BEGIN

/*!\plugin[perspectiverdist]{perspective\_rdist}{Perspective pinhole camera with radial distortion}
 * \order{8}
 * \parameters{
 *     \parameter{toWorld}{\Transform\Or\Animation}{
 *	      Specifies an optional camera-to-world transformation.
 *        \default{none (i.e. camera space $=$ world space)}
 *     }
 *     \parameter{kc}{\String}{
 *         Second and fourth-order coefficient of a polynomial radial distortion model
 *         specified as a comma-separated list
 *         The specifics of the model are described in detail on the following page:
 *         \url{http://www.vision.caltech.edu/bouguetj/calib_doc/htmls/parameters.html}
 *     }
 *     \parameter{focalLength}{\String}{
 *         Denotes the camera's focal length specified using
 *         \code{35mm} film equivalent units. See the main
 *         description for further details.
 *         \default{\code{50mm}}
 *     }
 *     \parameter{fov}{\Float}{
 *         An alternative to \code{focalLength}:
 *         denotes the camera's field of view in degrees---must be
 *         between 0 and 180, excluding the extremes.
 *     }
 *     \parameter{fovAxis}{\String}{
 *         When the parameter \code{fov} is given (and only then),
 *         this parameter further specifies the image axis, to
 *         which it applies.
 *         \begin{enumerate}[(i)]
 *             \item \code{\textbf{x}}: \code{fov} maps to the
 *                 \code{x}-axis in screen space.
 *             \item \code{\textbf{y}}: \code{fov} maps to the
 *                 \code{y}-axis in screen space.
 *             \item \code{\textbf{diagonal}}: \code{fov}
 *                maps to the screen diagonal.
 *             \item \code{\textbf{smaller}}: \code{fov}
 *                maps to the smaller dimension
 *                (e.g. \code{x} when \code{width}<\code{height})
 *             \item \code{\textbf{larger}}: \code{fov}
 *                maps to the larger dimension
 *                (e.g. \code{y} when \code{width}<\code{height})
 *         \end{enumerate}
 *         The default is \code{\textbf{x}}.
 *     }
 *     \parameter{shutterOpen, shutterClose}{\Float}{
 *         Specifies the time interval of the measurement---this
 *         is only relevant when the scene is in motion.
 *         \default{0}
 *     }
 *     \parameter{nearClip, farClip}{\Float}{
 *         Distance to the near/far clip
 *         planes.\default{\code{near\code}-\code{Clip=1e-2} (i.e.
 *         \code{0.01}) and {\code{farClip=1e4} (i.e. \code{10000})}}
 *     }
 * }
 *
 * This plugin extends the \pluginref{perspective} camera with support
 * for radial distortion. It accepts an additional parameter named \code{kc},
 * which specifies the second and fourth-order terms in a polynomial
 * model that accounts for pincushion and barrel distortion. This is
 * useful when trying to match rendered images to photographs created by a
 * camera whose distortion is known. When \code{kc=0, 0}, the model
 * turns into a standard pinhole camera. The reason for creating a separate
 * plugin for this feature is that distortion involves extra overheads per ray that
 * users may not be willing to pay for if their scene doesn't use it.
 * The MATLAB Camera Calibration Toolbox by Jean-Yves Bouguet
 * (\url{http://www.vision.caltech.edu/bouguetj/calib_doc/}) can be used to
 * obtain a distortion model, and the first entries of the \code{kc} variable
 * generated by this tool can directly be passed into this plugin.
 */

class PerspectiveCameraRDist : public PerspectiveCamera {
public:
	PerspectiveCameraRDist(const Properties &props)
			: PerspectiveCamera(props) {
		/* This sensor is the result of a limiting process where the aperture
		   radius tends to zero. However, it still has all the cosine
		   foreshortening terms caused by the aperture, hence the flag "EOnSurface" */
		m_type |= EDeltaPosition | EPerspectiveCamera | EOnSurface | EDirectionSampleMapsToPixels;

		if (props.getAnimatedTransform("toWorld", Transform())->eval(0).hasScale())
			Log(EError, "Scale factors in the camera-to-world "
				"transformation are not allowed!");

		/* Parse parameter for radial distortion model */
		std::string kc = props.getString("kc", "");
		std::vector<std::string> kc_tokens = tokenize(kc, ", ");
		if (kc_tokens.size() == 0) {
			m_kc[0] = m_kc[1] = 0.0f;
			m_distortion = false;
		} else if (kc_tokens.size() == 2) {
			char *end_ptr0, *end_ptr1;
			m_kc[0] = (Float) std::strtod(kc_tokens[0].c_str(), &end_ptr0);
			m_kc[1] = (Float) std::strtod(kc_tokens[1].c_str(), &end_ptr1);
			if (*end_ptr0 != '\0' || *end_ptr1 != 0)
				Log(EError, "Invalid input to the 'kc' parameter!");
			m_distortion = m_kc[0] != 0 || m_kc[1] != 0;
		} else {
			Log(EError, "The 'kc' requires two arguments!");
		}
	}

	PerspectiveCameraRDist(Stream *stream, InstanceManager *manager)
			: PerspectiveCamera(stream, manager) {
		configure();
		m_kc[0] = stream->readFloat();
		m_kc[1] = stream->readFloat();
		m_distortion = m_kc[0] != 0 || m_kc[1] != 0;
	}

	void serialize(Stream *stream, InstanceManager *manager) const {
		PerspectiveCamera::serialize(stream, manager);
		stream->writeFloat(m_kc[0]);
		stream->writeFloat(m_kc[1]);
	}

	void configure() {
		PerspectiveCamera::configure();

		const Vector2i &filmSize   = m_film->getSize();
		const Vector2i &cropSize   = m_film->getCropSize();
		const Point2i  &cropOffset = m_film->getCropOffset();

		Vector2 relSize((Float) cropSize.x / (Float) filmSize.x,
			(Float) cropSize.y / (Float) filmSize.y);
		Point2 relOffset((Float) cropOffset.x / (Float) filmSize.x,
			(Float) cropOffset.y / (Float) filmSize.y);

		/**
		 * These do the following (in reverse order):
		 *
		 * 1. Create transform from camera space to [-1,1]x[-1,1]x[0,1] clip
		 *    coordinates (not taking account of the aspect ratio yet)
		 *
		 * 2+3. Translate and scale to shift the clip coordinates into the
		 *    range from zero to one, and take the aspect ratio into account.
		 *
		 * 4+5. Translate and scale the coordinates once more to account
		 *     for a cropping window (if there is any)
		 */
		m_cameraToSample =
			  Transform::scale(Vector(1.0f / relSize.x, 1.0f / relSize.y, 1.0f))
			* Transform::translate(Vector(-relOffset.x, -relOffset.y, 0.0f))
			* Transform::scale(Vector(-0.5f, -0.5f*m_aspect, 1.0f))
			* Transform::translate(Vector(-1.0f, -1.0f/m_aspect, 0.0f))
			* Transform::perspective(m_xfov, m_nearClip, m_farClip);

		m_sampleToCamera = m_cameraToSample.inverse();

		/* Position differentials on the near plane */
		m_dx = m_sampleToCamera(Point(m_invResolution.x, 0.0f, 0.0f))
			 - m_sampleToCamera(Point(0.0f));
		m_dy = m_sampleToCamera(Point(0.0f, m_invResolution.y, 0.0f))
			 - m_sampleToCamera(Point(0.0f));

		/* Precompute some data for importance(). Please
		   look at that function for further details */
		Point min(m_sampleToCamera(Point(0, 0, 0))),
			  max(m_sampleToCamera(Point(1, 1, 0)));

		m_imageRect.reset();
		m_imageRect.expandBy(Point2(min.x, min.y) / min.z);
		m_imageRect.expandBy(Point2(max.x, max.y) / max.z);
		m_normalization = 1.0f / m_imageRect.getVolume();

		/* Clip-space transformation for OpenGL */
		m_clipTransform = Transform::translate(
			Vector((1-2*relOffset.x)/relSize.x - 1,
			      -(1-2*relOffset.y)/relSize.y + 1, 0.0f)) *
			Transform::scale(Vector(1.0f / relSize.x, 1.0f / relSize.y, 1.0f));
	}

	Float applyDistortion(Float r2) const {
		return 1 + r2*(m_kc[0] + r2*m_kc[1]);
	}

	Float invertDistortion(Float y) const {
		int it = 0;
		Float r = y;

		while (true) {
			Float r2 = r*r,
			      f  = r*(1+r2*(m_kc[0] + r2*m_kc[1])) - y,
			      df = 1 + r2*(3*m_kc[0] + 5*m_kc[1]*r2);

			r -= f / df;

			if (std::abs(f) < 1e-6 || ++it > 4)
				break;
		}
		return r/y;
	}

	/**
	 * \brief Compute the directional sensor response function
	 * of the camera multiplied with the cosine foreshortening
	 * factor associated with the image plane
	 *
	 * \param d
	 *     A normalized direction vector from the aperture position to the
	 *     reference point in question (all in local camera space)
	 */
	inline Float importance(const Vector &d) const {
		/* How is this derived? Imagine a hypothetical image plane at a
		   distance of d=1 away from the pinhole in camera space.

		   Then the visible rectangular portion of the plane has the area

		      A = (2 * tan(0.5 * xfov in radians))^2 / aspect

		   Since we allow crop regions, the actual visible area is
		   potentially reduced:

		      A' = A * (cropX / filmX) * (cropY / filmY)

		   Perspective transformations of such aligned rectangles produce
		   an equivalent scaled (but otherwise undistorted) rectangle
		   in screen space. This means that a strategy, which uniformly
		   generates samples in screen space has an associated area
		   density of 1/A' on this rectangle.

		   To compute the solid angle density of a sampled point P on
		   the rectangle, we can apply the usual measure conversion term:

		      d_omega = 1/A' * distance(P, origin)^2 / cos(theta)

		   where theta is the angle that the unit direction vector from
		   the origin to P makes with the rectangle. Since

		      distance(P, origin)^2 = Px^2 + Py^2 + 1

		   and

		      cos(theta) = 1/sqrt(Px^2 + Py^2 + 1),

		   we have

		      d_omega = 1 / (A' * cos^3(theta))
		*/

		Float cosTheta = Frame::cosTheta(d);

		/* Check if the direction points behind the camera */
		if (cosTheta <= 0)
			return 0.0f;

		/* Compute the position on the plane at distance 1 */
		Float invCosTheta = 1.0f / cosTheta;
		Point2 p(d.x * invCosTheta, d.y * invCosTheta);

		Float importance = m_normalization * invCosTheta
				* invCosTheta * invCosTheta;

		if (m_distortion) {
			/* Correct importance for radial distortion */
			Float rOrig2  = Vector2(p).lengthSquared(),
				  rFactor = applyDistortion(rOrig2);

			Float deriv = 1 + rOrig2*(3*m_kc[0] + 5*m_kc[1]*rOrig2);
			importance *= std::abs(rFactor*deriv);
			p *= rFactor;
		}

		/* Check if the point lies inside the chosen crop rectangle */
		if (!m_imageRect.contains(p))
			return 0.0f;

		return importance;
	}

	Spectrum sampleRay(Ray &ray, const Point2 &pixelSample,
			const Point2 &otherSample, Float timeSample) const {
		ray.time = sampleTime(timeSample);

		/* Compute the corresponding position on the
		   near plane (in local camera space) */
		Point nearP = m_sampleToCamera(Point(
			pixelSample.x * m_invResolution.x,
			pixelSample.y * m_invResolution.y, 0.0f));

		if (m_distortion) {
			Float correction = invertDistortion(Vector2(nearP.x / nearP.z, nearP.y / nearP.z).length());
			nearP.x *= correction; nearP.y *= correction;
		}

		/* Turn that into a normalized ray direction, and
		   adjust the ray interval accordingly */
		Vector d = normalize(Vector(nearP));
		Float invZ = 1.0f / d.z;

		ray.mint = m_nearClip * invZ;
		ray.maxt = m_farClip * invZ;

		Point2 p(d.x * invZ, d.y * invZ);

		const Transform &trafo = m_worldTransform->eval(ray.time);
		ray.setOrigin(trafo.transformAffine(Point(0.0f)));
		ray.setDirection(trafo(d));

		return Spectrum(1.0f);
	}

	Spectrum sampleRayDifferential(RayDifferential &ray, const Point2 &pixelSample,
			const Point2 &otherSample, Float timeSample) const {
		ray.time = sampleTime(timeSample);

		/* Compute the corresponding position on the
		   near plane (in local camera space) */
		Point nearP = m_sampleToCamera(Point(
			pixelSample.x * m_invResolution.x,
			pixelSample.y * m_invResolution.y, 0.0f));

		if (m_distortion) {
			/* Ray differentials don't take distortion into account */
			Float correction = invertDistortion(Vector2(nearP.x / nearP.z, nearP.y / nearP.z).length());
			nearP.x *= correction; nearP.y *= correction;
		}

		/* Turn that into a normalized ray direction, and
		   adjust the ray interval accordingly */
		Vector d = normalize(Vector(nearP));
		Float invZ = 1.0f / d.z;
		ray.mint = m_nearClip * invZ;
		ray.maxt = m_farClip * invZ;

		const Transform &trafo = m_worldTransform->eval(ray.time);
		ray.setOrigin(trafo.transformAffine(Point(0.0f)));
		ray.setDirection(trafo(d));
		ray.rxOrigin = ray.ryOrigin = ray.o;

		ray.rxDirection = trafo(normalize(Vector(nearP) + m_dx));
		ray.ryDirection = trafo(normalize(Vector(nearP) + m_dy));
		ray.hasDifferentials = true;

		return Spectrum(1.0f);
	}

	Spectrum samplePosition(PositionSamplingRecord &pRec,
			const Point2 &sample, const Point2 *extra) const {
		const Transform &trafo = m_worldTransform->eval(pRec.time);
		pRec.p = trafo(Point(0.0f));
		pRec.n = trafo(Vector(0.0f, 0.0f, 1.0f));
		pRec.pdf = 1.0f;
		pRec.measure = EDiscrete;
		return Spectrum(1.0f);
	}

	Spectrum evalPosition(const PositionSamplingRecord &pRec) const {
		return Spectrum((pRec.measure == EDiscrete) ? 1.0f : 0.0f);
	}

	Float pdfPosition(const PositionSamplingRecord &pRec) const {
		return (pRec.measure == EDiscrete) ? 1.0f : 0.0f;
	}

	Spectrum sampleDirection(DirectionSamplingRecord &dRec,
			PositionSamplingRecord &pRec,
			const Point2 &sample, const Point2 *extra) const {
		const Transform &trafo = m_worldTransform->eval(pRec.time);

		Point samplePos(sample.x, sample.y, 0.0f);

		if (extra) {
			/* The caller wants to condition on a specific pixel position */
			samplePos.x = (extra->x + sample.x) * m_invResolution.x;
			samplePos.y = (extra->y + sample.y) * m_invResolution.y;
		}

		pRec.uv = Point2(samplePos.x * m_resolution.x,
			samplePos.y * m_resolution.y);

		/* Compute the corresponding position on the
		   near plane (in local camera space) */
		Point nearP = m_sampleToCamera(samplePos);

		/* Turn that into a normalized ray direction */
		Vector d = normalize(Vector(nearP));
		dRec.d = trafo(d);
		dRec.measure = ESolidAngle;
		dRec.pdf = m_normalization / (d.z * d.z * d.z);

		return Spectrum(1.0f);
	}

	Float pdfDirection(const DirectionSamplingRecord &dRec,
			const PositionSamplingRecord &pRec) const {
		if (dRec.measure != ESolidAngle)
			return 0.0f;

		const Transform &trafo = m_worldTransform->eval(pRec.time);

		return importance(trafo.inverse()(dRec.d));
	}

	Spectrum evalDirection(const DirectionSamplingRecord &dRec,
			const PositionSamplingRecord &pRec) const {
		if (dRec.measure != ESolidAngle)
			return Spectrum(0.0f);

		const Transform &trafo = m_worldTransform->eval(pRec.time);

		return Spectrum(importance(trafo.inverse()(dRec.d)));
	}

	bool getSamplePosition(const PositionSamplingRecord &pRec,
			const DirectionSamplingRecord &dRec, Point2 &samplePosition) const {
		Transform invTrafo = m_worldTransform->eval(pRec.time).inverse();
		Point local(Point(invTrafo(dRec.d)));

		if (m_distortion) {
			Float correction = applyDistortion(Vector2(local.x / local.z, local.y / local.z).lengthSquared());
			local.x *= correction; local.y *= correction;
		}

		if (local.z <= 0)
			return false;

		Point screenSample = m_cameraToSample(local);
		if (screenSample.x < 0 || screenSample.x > 1 ||
			screenSample.y < 0 || screenSample.y > 1)
			return false;

		samplePosition = Point2(
				screenSample.x * m_resolution.x,
				screenSample.y * m_resolution.y);

		return true;
	}

	Spectrum sampleDirect(DirectSamplingRecord &dRec, const Point2 &sample) const {
		const Transform &trafo = m_worldTransform->eval(dRec.time);

		/* Transform the reference point into the local coordinate system */
		Point refP = trafo.inverse().transformAffine(dRec.ref);

		/* Check if it is outside of the clip range */
		if (refP.z < m_nearClip || refP.z > m_farClip) {
			dRec.pdf = 0.0f;
			return Spectrum(0.0f);
		}

		Point refPc(refP);

		if (m_distortion) {
			Float correction = applyDistortion(Vector2(refPc.x / refPc.z, refPc.y / refPc.z).lengthSquared());
			refPc.x *= correction; refPc.y *= correction;
		}

		Point screenSample = m_cameraToSample(refPc);
		dRec.uv = Point2(screenSample.x, screenSample.y);
		if (dRec.uv.x < 0 || dRec.uv.x > 1 ||
			dRec.uv.y < 0 || dRec.uv.y > 1) {
			dRec.pdf = 0.0f;
			return Spectrum(0.0f);
		}

		dRec.uv.x *= m_resolution.x;
		dRec.uv.y *= m_resolution.y;

		Vector localD(refP);
		Float dist = localD.length(),
			  invDist = 1.0f / dist;
		localD *= invDist;

		dRec.p = trafo.transformAffine(Point(0.0f));
		dRec.d = (dRec.p - dRec.ref) * invDist;
		dRec.dist = dist;
		dRec.n = trafo(Vector(0.0f, 0.0f, 1.0f));
		dRec.pdf = 1;
		dRec.measure = EDiscrete;

		return Spectrum(
			importance(localD) * invDist * invDist);
	}

	Float pdfDirect(const DirectSamplingRecord &dRec) const {
		return (dRec.measure == EDiscrete) ? 1.0f : 0.0f;
	}

	Transform getProjectionTransform(const Point2 &apertureSample,
			const Point2 &aaSample) const {
		Float right = std::tan(m_xfov * M_PI/360) * m_nearClip, left = -right;
		Float top = right / m_aspect, bottom = -top;

		Vector2 offset(
			(right-left)/m_film->getSize().x * (aaSample.x-0.5f),
			(top-bottom)/m_film->getSize().y * (aaSample.y-0.5f));

		return m_clipTransform *
			Transform::glFrustum(left+offset.x, right+offset.x,
				bottom+offset.y, top+offset.y, m_nearClip, m_farClip);
	}

	AABB getAABB() const {
		return m_worldTransform->getTranslationBounds();
	}

	std::string toString() const {
		std::ostringstream oss;
		oss << "PerspectiveCameraRDist[" << endl
			<< "  xfov = " << m_xfov << "," << endl
			<< "  nearClip = " << m_nearClip << "," << endl
			<< "  farClip = " << m_farClip << "," << endl
			<< "  worldTransform = " << indent(m_worldTransform.toString()) << "," << endl
			<< "  sampler = " << indent(m_sampler->toString()) << "," << endl
			<< "  film = " << indent(m_film->toString()) << "," << endl
			<< "  medium = " << indent(m_medium.toString()) << "," << endl
			<< "  shutterOpen = " << m_shutterOpen << "," << endl
			<< "  shutterOpenTime = " << m_shutterOpenTime << endl
			<< "]";
		return oss.str();
	}

	MTS_DECLARE_CLASS()
private:
	Transform m_cameraToSample;
	Transform m_sampleToCamera;
	Transform m_clipTransform;
	AABB2 m_imageRect;
	Float m_normalization;
	Vector m_dx, m_dy;
	bool m_distortion;
	Float m_kc[2];
};

MTS_IMPLEMENT_CLASS_S(PerspectiveCameraRDist, false, PerspectiveCamera)
MTS_EXPORT_PLUGIN(PerspectiveCameraRDist, "Perspective camera with radial distortion");
MTS_NAMESPACE_END
