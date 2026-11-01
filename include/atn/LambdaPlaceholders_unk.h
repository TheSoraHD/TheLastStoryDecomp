#ifndef ATN_LAMBDAPLACEHOLDERS_UNK_H
#define ATN_LAMBDAPLACEHOLDERS_UNK_H

// inferred: the boost::lambda placeholders of boost/lambda/core.hpp (free1..3 and the references
// _1.._3 in an unnamed namespace): every file that includes the header gets six .sbss objects and a
// __sinit that sets up the references (atn/SequenceHolder.cpp, gmk/CharaLight.cpp). The boost::lambda
// headers are not vendored; the types are empty stand-ins.
struct LambdaPlaceholder1_unk {};
struct LambdaPlaceholder2_unk {};
struct LambdaPlaceholder3_unk {};
namespace {
LambdaPlaceholder1_unk free1;
LambdaPlaceholder2_unk free2;
LambdaPlaceholder3_unk free3;
LambdaPlaceholder1_unk& _1 = free1;
LambdaPlaceholder2_unk& _2 = free2;
LambdaPlaceholder3_unk& _3 = free3;
} // namespace

#endif
