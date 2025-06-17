#pragma once
#ifdef __cplusplus
extern "C" {
#endif
typedef struct _SatEngine SatEngine;
typedef enum EncodingType {
  SAT_ENCODING_NONE,
  SAT_ENCODING_PAIRWISE,
  SAT_ENCODING_TREE,
  SAT_ENCODING_AUTO
} EncodingType;

SatEngine* satCreate();
void       satAdd(SatEngine* engine, int variable);
void       satGenVariables(SatEngine* engine, int* variables, int count);
void       satAtMostOne(SatEngine* engine, int* variables, int count, EncodingType encoding);
void       satAtLeastOne(SatEngine* engine, int* variables, int count);
void       satExactlyOne(SatEngine* engine, int* variables, int count, EncodingType encoding);
int        satSolve(SatEngine* engine);
int        satGetValue(SatEngine* engine, int variable);
void       satDispose(SatEngine* engine);

#ifdef __cplusplus
}
#endif
