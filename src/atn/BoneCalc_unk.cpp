// atn/BoneCalc_unk.cpp: calculo recursivo de las matrices de los huesos de un modelo (nombre de fichero
// inventado). Solo esta escrita la funcion vacia que usan como puntero a miembro las tres constantes
// de .data (0x80778930: {0, -1, fn_8009B938}).
//
// Pendiente (no escritas): fn_8009A5CC (0x7D8, llamada por ModelPose_unk::fn_8009A584) y
// fn_8009ADA4 (0xB94, llamada por atn::PBoneBasic::vf10): recorren los nodos con un
// boost::function<> local y punteros a miembro, convierten angulos de Euler con fn_8072D210
// (seno/coseno de tabla, escala 128/pi y medio angulo) y concatenan matrices con PSMTXConcat.

// inferred: empty callback (the target of the pointer-to-member constants of this file)
extern "C" void fn_8009B938() {}
