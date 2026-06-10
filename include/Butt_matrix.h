#ifndef B_MATRIX_H
#define B_MATRIX_H

void Handle_T1();
void Handle_T2();
void Handle_T3();

void Handle_M1();
void Handle_M2();
void Handle_M3();

void Handle_B1();
void Handle_B2();
void Handle_B3();


void run_matrix(void (*call_inp)() = nullptr);
void run_matrix_def();

extern void (*call_mat)();


#endif