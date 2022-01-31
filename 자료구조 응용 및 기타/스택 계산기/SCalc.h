#ifndef _SCALC_H_
#define _SCALC_H_

double CalcOperation(double, char, double);
double CalcOperation(double, SYMBOL_TYPE, double);

void GenPostfixExpr(const char*, char*);
double CalcPostfixExpr(const char*);
#endif