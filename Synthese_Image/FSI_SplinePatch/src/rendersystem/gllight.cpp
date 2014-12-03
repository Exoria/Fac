#include "gllight.h"
namespace rendersystem {

MyGlLight::MyGlLight(const glm::vec3 &pos, const glm::vec3 &col, bool sc) : position(pos), color(col), shadowCaster(sc) {
}
const glm::vec3 &MyGlLight::getPosition(){
    return position;
}

const glm::vec3 &MyGlLight::getColor(){
    return color;
}

bool MyGlLight::castShadows(){
    return shadowCaster;
}


}
