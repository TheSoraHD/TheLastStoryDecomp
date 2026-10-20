// atn/Math_unk.cpp: constantes globales de matematicas del motor (nombre de fichero inventado;
// probablemente atn_math.cpp). Solo contiene el __sinit que las construye.
#include "math/Vec.h"

// inferred: vector 2D cero, vector 3D cero, vector 4D cero, cuaternion identidad y matriz identidad
// (orden de los stores del __sinit 0x80081DC8; los nombres reales son desconocidos)
extern "C" {
atn::Vector2_unk lbl_807C7028(0.0f, 0.0f);
atn::Vector3 lbl_807C7030(0.0f, 0.0f, 0.0f);
atn::Quaternion_unk lbl_807C7040(0.0f, 0.0f, 0.0f, 0.0f);
atn::Quaternion_unk lbl_807C7050(0.0f, 0.0f, 0.0f, 1.0f);
atn::Matrix3x4 lbl_807C7060(1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f);
}
