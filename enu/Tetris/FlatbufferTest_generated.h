// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_FLATBUFFERTEST_FBE_H_
#define FLATBUFFERS_GENERATED_FLATBUFFERTEST_FBE_H_

#include "flatbuffers/flatbuffers.h"

namespace fbe {

enum Color {
  Color_Red = 0,
  Color_Green = 1,
  Color_Blue = 2,
  Color_MIN = Color_Red,
  Color_MAX = Color_Blue
};

inline const Color (&EnumValuesColor())[3] {
  static const Color values[] = {
    Color_Red,
    Color_Green,
    Color_Blue
  };
  return values;
}

inline const char * const *EnumNamesColor() {
  static const char * const names[] = {
    "Red",
    "Green",
    "Blue",
    nullptr
  };
  return names;
}

inline const char *EnumNameColor(Color e) {
  const size_t index = static_cast<int>(e);
  return EnumNamesColor()[index];
}

}  // namespace fbe

#endif  // FLATBUFFERS_GENERATED_FLATBUFFERTEST_FBE_H_
