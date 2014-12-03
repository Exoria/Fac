#version 150
const float globalColorFactor = 0.75;

// material
uniform vec3 materialKd;
uniform vec3 materialKs;
uniform float materialNs;

// texture
uniform sampler2D baseTexture;

// fragment
in vec3 varNormal;
in vec4 varTexCoord;

// lumieres
in vec3 lightDirInView[3];
in vec3 halfVecInView[3];

// material
uniform vec3 keyLightColor;
uniform vec3 fillLightColor;
uniform vec3 backLightColor;

// resultat
out vec4 outColor;

// --------------------

vec3 blinnPhongLighting (in vec3 kd, in vec3 ks, in float ns, in vec3 normal, in vec3 lightVector, in vec3 halfVector){
    // Fonction à écrire par les étudiants

    halfVector = normalize(halfVector + lightVector);

    float costetha = clamp(dot(normal, lightVector), 0., 1.);
/*    if(costetha <0.33) costetha = 0.;
    else if(costetha <0.66) costetha = 0.5;
    else costetha=1;
  */
    float cosalphan = pow( clamp( dot(halfVector,normal), 0., 1.), ns );
    //cosalphan = step(0.8,cosalphan);
    vec3 result = (kd + (ks* cosalphan)) * costetha;
    return result;
}

vec2 blinnPhongLightingF (in float ns, in vec3 normal, in vec3 lightVector, in vec3 halfVector){
    // Fonction à écrire par les étudiants

    halfVector = normalize(halfVector + lightVector);

    float costetha = clamp(dot(normal, lightVector), 0., 1.);
    float cosalphan = pow( clamp( dot(halfVector,normal), 0., 1.), ns );

    return vec2(costetha, cosalphan);
}


void main(void) {

    vec3 fragColor = vec3(0.0, 0.0, 0.0);
//    fragColor = blinnPhongLighting(materialKd, materialKs, materialNs, normalize(varNormal), normalize(lightDirInView[0]), normalize(halfVecInView[0]));

    //vec3 kd = materialKd;
    vec3 kd = texture(baseTexture, varTexCoord.st).rgb;




    fragColor = fragColor + keyLightColor*blinnPhongLighting(kd, materialKs, materialNs, normalize(varNormal), normalize(lightDirInView[0]), normalize(halfVecInView[0]));
    fragColor = fragColor + fillLightColor*blinnPhongLighting(kd, materialKs, materialNs, normalize(varNormal), normalize(lightDirInView[1]), normalize(halfVecInView[1]));
    fragColor = fragColor + backLightColor*blinnPhongLighting(kd, materialKs, materialNs, normalize(varNormal), normalize(lightDirInView[2]), normalize(halfVecInView[2]));

/*
    vec2 intens = blinnPhongLightingF( materialNs, normalize(varNormal), normalize(lightDirInView[0]), normalize(halfVecInView[0]));
    intens += blinnPhongLightingF( materialNs, normalize(varNormal), normalize(lightDirInView[1]), normalize(halfVecInView[1]));
    intens += blinnPhongLightingF( materialNs, normalize(varNormal), normalize(lightDirInView[2]), normalize(halfVecInView[2]));
    //intens.x/=3;
    if(intens.x <0.33) intens.x = 0.;
    else if(intens.x <0.66) intens.x = 0.5;
    else intens.x=1;
*/


    outColor = vec4( fragColor*globalColorFactor, 1.);

    //outColor = vec4(kd, 1.);
/*
    intens.y = step(0.5, intens.y);
    fragColor = intens.x*(kd*keyLightColor + kd*fillLightColor + kd*backLightColor)/3.0 + intens.y;
*/
    //outColor = vec4( globalColorFactor*fragColor, 1.);
    //outColor = vec4( fragColor, 1.);
//    outColor = vec4(backLightColor, 1.);
}

