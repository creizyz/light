#include <light.hpp>

namespace light {

Light::Light() {
    number = 6;
}

int Light::get_number() const {
  return number;
}

}
