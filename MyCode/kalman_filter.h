#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

#include "main.h"
#include "arm_math.h"
#include "cmsis_os.h"

#define mat         arm_matrix_instance_f32 
#define mat_init    arm_mat_init_f32
#define mat_add     arm_mat_add_f32
#define mat_sub     arm_mat_sub_f32
#define mat_mult    arm_mat_mult_f32
#define mat_trans   arm_mat_trans_f32
#define mat_inv     arm_mat_inverse_f32


//#define T 0.001f
//#define qqx 1.0f
//#define qqy 1.0f

#define CH_COUNT 8




typedef struct
{
  float raw_value;
  float filtered_value[2];
  mat xhat, xhatminus, z, A, H, AT, HT, Q, R, P, Pminus, K;
} kalman_filter_t;

typedef struct
{
  float raw_value;
  float filtered_value[2];
	float A_data[16];
	float AT_data[16];
	float Q_data[16];
	float H_data[8];
	float HT_data[8];
	float xhat_data[4];
	float xhatminus_data[4];
	float Pminus_data[16];
	float P_data[16];
	float K_data[8];
	float z_data[2];
	float R_data[4];
} kalman_filter_init_t;


void KF_para_ahead_location(kalman_filter_init_t* KFinit);
float *kalman_filter_calc(kalman_filter_t *F, float signal1, float signal2);
void kalman_tracking_task(void const * argument);
extern kalman_filter_t KF_ahead_location;
extern kalman_filter_init_t ahead_location_kfinit;
void kalman_filter_init(kalman_filter_t* KF,kalman_filter_init_t* KFinit);


#endif

