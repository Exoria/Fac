#version 150
const float globalColorFactor = 0.75;

// material
uniform vec3 materialKd;
uniform vec3 materialKs;
uniform float materialNs;

// fragment
in vec3 varNormal;
in vec4 varTexCoord;

// lumieres
in vec3 lightDirInView[4];
in vec3 halfVecInView[4];

// material
uniform vec3 keyLightColor;
uniform vec3 fillLightColor;
uniform vec3 backLightColor;

// resultat
out vec4 outColor;

// --------------------

vec3 blinnPhongLighting (in vec3 kd, in vec3 ks, in float ns, in vec3 normal, in vec3 lightVector, in vec3 halfVector){
    // Fonction à écrire par les étudiants

    float costetha = clamp(dot(normal, lightVector), 0., 1.);
    float cosalphan = pow( clamp( dot(halfVector,normal), 0., 1.), ns );
    vec3 result = (kd + (ks* cosalphan)) * costetha;
    return result;
}

void main(void) {

    vec3 fragColor = vec3(0.0, 0.0, 0.0);
    //fragColor = blinnPhongLighting(materialKd, materialKs, materialNs, normalize(varNormal), normalize(lightDirInView[0]), normalize(halfVecInView[0]));


    fragColor = fragColor + keyLightColor*blinnPhongLighting(materialKd, materialKs, materialNs, normalize(varNormal), normalize(lightDirInView[0]), normalize(halfVecInView[0]));
    fragColor = fragColor + fillLightColor*blinnPhongLighting(materialKd, materialKs, materialNs, normalize(varNormal), normalize(lightDirInView[1]), normalize(halfVecInView[1]));
    fragColor = fragColor + backLightColor*blinnPhongLighting(materialKd, materialKs, materialNs, normalize(varNormal), normalize(lightDirInView[2]), normalize(halfVecInView[2]));
    fragColor = fragColor + keyLightColor*blinnPhongLighting(materialKd, materialKs, materialNs, normalize(varNormal), normalize(lightDirInView[3]), normalize(halfVecInView[3]));

    outColor = vec4( fragColor*globalColorFactor, 1.);

}

