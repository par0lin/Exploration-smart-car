#include "kalman_filter.h"
#include "string.h"



kalman_filter_t KF_ahead_location;
kalman_filter_init_t ahead_location_kfinit;



//	float r=0.01f;
//	float qqx=0.75f;
//	float qqy=0.75f;

	

void KF_para_ahead_location(kalman_filter_init_t* KFinit)
{
	float r=0.1f;
	float qqx=0.0000001f;
	float qqy=0.0000001f;
	float T=0.002f;
	float A_data[16]={1,T,0,0,0,1,0,0,0,0,1,T,0,0,0,1};
	float AT_data[16]={1,0,0,0,T,1,0,0,0,0,1,0,0,0,T,1};
	float Q_data[16]={T*T*T/3*qqx,T*T/2*qqx,0,0,T*T/2*qqx,T*qqx,0,0,0,0,T*T*T/3*qqy,T*T/2*qqy,0,0,T*T/2*qqy,T*qqy};
	float H_data[8]={1,0,0,0,0,0,1,0};
	float HT_data[8]={1,0,0,0,0,1,0,0};
	float xhat_data[4]={0,0,0,0};
	float xhatminus_data[4]={0,0,0,0};
	float Pminus_data[16]={1000,0,0,0,0,1000,0,0,0,0,1000,0,0,0,0,1000};
	float P_data[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	float K_data[8]={0,0,0,0,0,0,0,0};
	float z_data[2]={0,0};
	float R_data[4]={r,0,0,r};
	memcpy(KFinit->A_data,A_data,sizeof(A_data));
	memcpy(KFinit->AT_data,AT_data,sizeof(AT_data));
	memcpy(KFinit->Q_data,Q_data,sizeof(Q_data));
	memcpy(KFinit->H_data,H_data,sizeof(H_data));
	memcpy(KFinit->HT_data,HT_data,sizeof(HT_data));
	memcpy(KFinit->xhat_data,xhat_data,sizeof(xhat_data));
	memcpy(KFinit->xhatminus_data,xhatminus_data,sizeof(xhatminus_data));
	memcpy(KFinit->Pminus_data,Pminus_data,sizeof(Pminus_data));
	memcpy(KFinit->P_data,P_data,sizeof(P_data));
	memcpy(KFinit->K_data,K_data,sizeof(K_data));
	memcpy(KFinit->z_data,z_data,sizeof(z_data));
	memcpy(KFinit->R_data,R_data,sizeof(R_data));
}


void kalman_tracking_task(void const * argument)
{
	// KF_para_ahead_location(&ahead_location_kfinit)
	// kalman_filter_init(&KF_ahead_location,&ahead_location_kfinit);
	while(1)
	{


		
//		if(Autoaim_valid())kalman_filter_calc(&receive_y,selfaim_receive.ylocation,1);
//		if(Autoaim_valid())kalman_filter_calc(&receive_z,selfaim_receive.zlocation,1);
		
//		Vofa_Send_Data(&Vofa_data,receive_z.xhat.pData[0],selfaim_receive.zlocation);
		osDelay(1);			
	}
}




		float TEMP_data44[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		float TEMP_data24[8] = {0,0,0,0,0,0,0,0};
		float TEMP_data42[8] = {0,0,0,0,0,0,0,0};
		float TEMP_data22[4] = {0,0,0,0};
		float TEMP_data21[2] = {0,0};
		float S_data[4] = {0,0,0,0};
		
		mat TEMP44,TEMP24,TEMP42,TEMP22,TEMP21,S;


void kalman_filter_init(kalman_filter_t* KF,kalman_filter_init_t* KFinit)
{

		mat_init(&KF->A,4,4,KFinit->A_data);
		mat_init(&KF->AT,4,4,KFinit->AT_data);
		//	mat_trans(&KF->A, &KF->AT);
		mat_init(&KF->H,2,4,KFinit->H_data);
		mat_init(&KF->HT,4,2,KFinit->HT_data);
		//	mat_trans(&KF->H, &KF->HT);
		mat_init(&KF->Q,4,4,KFinit->Q_data);
		mat_init(&KF->xhat,4,1,KFinit->xhat_data);
		mat_init(&KF->xhatminus,4,1,KFinit->xhatminus_data);
		mat_init(&KF->Pminus,4,4,KFinit->Pminus_data);
		mat_init(&KF->P,4,4,KFinit->P_data);
		mat_init(&KF->K,4,2,KFinit->K_data);
		mat_init(&KF->R,2,2,KFinit->R_data);
		mat_init(&KF->z,2,1,KFinit->z_data);



		mat_init(&TEMP44,4,4,TEMP_data44);
		mat_init(&TEMP24,2,4,TEMP_data24);
		mat_init(&TEMP42,4,2,TEMP_data42);
		mat_init(&TEMP22,2,2,TEMP_data22);
		mat_init(&TEMP21,2,1,TEMP_data21);
		mat_init(&S,2,2,S_data);
}


	


float *kalman_filter_calc(kalman_filter_t *F, float signal1, float signal2)
{


  F->z.pData[0] = signal1;
  F->z.pData[1] = signal2;
  //1. xhat'(k)= A xhat(k-1)
  mat_mult(&F->A, &F->xhat, &F->xhatminus);

  //2. P'(k) = A P(k-1) AT + Q
  mat_mult(&F->A, &F->P, &F->Pminus);
  mat_mult(&F->Pminus, &F->AT, &F->Pminus);
  mat_add(&F->Pminus, &F->Q, &F->Pminus);

  //3. K(k) = P'(k) HT / (H P'(k) HT + R)
  mat_mult(&F->H, &F->Pminus, &TEMP24);
  mat_mult(&TEMP24, &F->HT, &TEMP22);
  mat_add(&TEMP22, &F->R, &S);//S = H P'(k) HT + R

  mat_inv(&S, &TEMP22);
  mat_mult(&F->Pminus, &F->HT, &TEMP42);
  mat_mult(&TEMP42, &TEMP22, &F->K);


  //4. xhat(k) = xhat'(k) + K(k) (z(k) - H xhat'(k))
  mat_mult(&F->H, &F->xhatminus, &TEMP21);
  mat_sub(&F->z, &TEMP21, &F->z);
  mat_mult(&F->K, &F->z, &F->xhat);
  mat_add(&F->xhatminus, &F->xhat, &F->xhat);

  //5. P(k) = (1-K(k)H)P'(k) ==> P(k) = P'(k)-K(k)¡¤H¡¤P'(k)
  mat_mult(&F->K, &F->H, &TEMP44);               
  mat_mult(&TEMP44, &F->Pminus, &TEMP44); 
  mat_sub(&F->Pminus, &TEMP44, &F->P);

  F->filtered_value[0] = F->xhat.pData[0];
  F->filtered_value[1] = F->xhat.pData[1];

  return F->filtered_value;
}


