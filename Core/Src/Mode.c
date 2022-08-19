/*
 * MouseMode.c
 *
 *  Created on: Feb 17, 2022
 *      Author: leopi
 */
#include <action.h>
#include "MicroMouse.h"
#include "Mode.h"

#include <stdio.h>

#include "PID_Control.h"
#include "Convert.h"

#include "IEH2_4096.h"
#include "ADC.h"
#include "LED_Driver.h"
#include "IR_Emitter.h"
#include "Motor_Driver.h"
#include "ICM_20648.h"
#include "UI.h"
#include "Map.h"
#include "Search.h"
#include "Flash.h"
#include "Interrupt.h"
#include "Debug.h"

#include <main.h>
#include "CommandQueue.h"
void InitExplore()
{
#if 0
	//ペリフェラルの動作開始
	Motor_PWM_Start();
	EncoderStart();
	EmitterON();
	ADCStart();
	IMU_init();

	PIDReset(L_VELO_PID);
	PIDReset(R_VELO_PID);
	//PIDReset(B_VELO);
	PIDReset(D_WALL_PID);
	PIDReset(A_VELO_PID);

	//PID制御を有効化
	PIDChangeFlag(L_VELO_PID, 1);
	PIDChangeFlag(R_VELO_PID, 1);
	PIDChangeFlag(D_WALL_PID, 0);
	//PIDChangeFlag(B_VELO, 0);
	PIDChangeFlag(A_VELO_PID, 0);

	PIDSetGain(L_VELO_PID, 1.9613, 50.3786, 0.011499);//1.5011, 38.632, 0.0106);//2.4471, 58.7382, 0.015592);//3.4933, 82.6932, 0.017737);//13.5666, 524.3235, 0.064722);//50.4479, 2811.5036, 0.2263);//1.1941, 33.5232, 0.0059922);
	PIDSetGain(R_VELO_PID, 1.9613, 50.3786, 0.011499);// 1.5011, 38.632, 0.0106);//2.4471, 58.7382, 0.015592);//3.4933, 82.6932, 0.017737);//13.5666, 524.3235, 0.064722);//50.4479, 2811.5036, 0.2263);//1.1941, 33.5232, 0.0059922);
	//PIDSetGain(B_VELO, 1.1941, 33.5232, 0.0059922);
	PIDSetGain(A_VELO_PID, 28.6379,340.0855,0.21289);//17.4394, 321.233, 0.12492);
	PIDSetGain(D_WALL_PID, 2, 0.1, 0.00004);
	InitPulse( (int*)(&(TIM3->CNT)),  INITIAL_PULSE);
	InitPulse( (int*)(&(TIM4->CNT)),  INITIAL_PULSE);

	//割り込みを有効化
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim8);
	//ここまででハードの準備はできた。
	//ここからはソフト的な準備
	TargetVelocityBody = 0;
	TargetAngularV = 0;
	Acceleration = 0;
	AngularAcceleration = 0;
	TotalPulse[LEFT] = 0;
	TotalPulse[RIGHT] = 0;
	TotalPulse[BODY] = 0;

	//両壁の値を取得。それぞれの値と差分を制御目標に反映。
	IMU_Calib();
	TargetPhoto[SL] = Photo[SL];
	TargetPhoto[SR] = Photo[SR];
	PhotoDiff = TargetPhoto[SL] - TargetPhoto[SR];

	PIDReset(L_VELO_PID);
	PIDReset(R_VELO_PID);
	//PIDReset(B_VELO);
	PIDReset(D_WALL_PID);
	PIDReset(A_VELO_PID);

	HAL_Delay(500);
#else

//	htim2.Init.Prescaler = 20-1;
//	for(int i=0; i < 9 ; i++)
//	{
//		  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
//		  {
//		    Error_Handler();
//		  }
//		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 1000);
//		HAL_Delay(500);
//		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);
//		HAL_Delay(500);
//		htim2.Init.Prescaler -= 2;
//
//	}
	//IMU_DMA_Start();
	//CS_RESET;

	//PID制御準備
	//PIDInit();
	PIDChangeFlag(L_VELO_PID, 0);
	PIDChangeFlag(R_VELO_PID, 0);
	PIDChangeFlag(L_WALL_PID, 0);
	PIDChangeFlag(R_WALL_PID, 0);
	PIDChangeFlag(D_WALL_PID, 0);
	//PIDChangeFlag(B_VELO, 0);
	PIDChangeFlag(A_VELO_PID, 0);


	Load_Gain();
	uint8_t imu_check;
	imu_check = IMU_init();
	printf("imu_check 1ならOK: %d\r\n",imu_check);
#if 1 //IMUから値が来なくなる現象の対策
	imu_check =IMU_init();
	printf("imu_check 1ならOK: %d\r\n",imu_check);
#endif
	HAL_Delay(100);

	ZGyro = ReadIMU(0x37, 0x38);
	printf("gyro : %f\r\n",ZGyro);


#if 0
	wall_init();
	//ジャイロの読み取りにかかる時間の計測
//  int16_t data[1000]={0};
//  int i=0, elaps=0;

  HAL_TIM_Base_Start_IT(&htim8);
t = 1;
//	wall_set();//現在座標じゃなくて、進行方向から求めた次の座標。
//	//計算して
//	UpdateWalkMap();
  make_map(X_GOAL_LESSER, Y_GOAL_LESSER, 0x01); //0.05*7=0.35msで済んだ。
  t = 0;
  HAL_TIM_Base_Stop_IT(&htim8);
  //data[i] = zg;
  while(1)
  {
	  printf("timer8 : %d\r\n",timer8);
	  map_print();
//	  t = 1;
//	  //read_gyro_data();
//	  ZGyro = ReadByte();
//	  t = 0;
//	  HAL_TIM_Base_Stop_IT(&htim8);
//	  data[i] = zg;
//	  i++;
//	  zg = 0;
//	  if(i == 1000)
//	  {
//		  t = 0;
//		  HAL_TIM_Base_Stop_IT(&htim8);
//		  break;
//	  }

  }
#endif
	//ペリフェラルの動作開始
	Motor_PWM_Start();
	EncoderStart(); //戻し忘れないように
	EmitterON();
	ADCStart();
	InitPulse( (int*)(&(TIM3->CNT)),  INITIAL_PULSE);
	InitPulse( (int*)(&(TIM4->CNT)),  INITIAL_PULSE);
	//割り込みを有効化
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim8);
	//ここまででハードの準備はできた。
	//ここからはソフト的な準備

#if 0
	while(1)
	{
		printf("生値%f, 加速度%f g, %f mm/s\r\n",YAccel,YAccel/2048.0f, ImuAccel);
		printf("生値%f, 角速度%f ang/s, %f rad/s\r\n",ZGyro,ZGyro/16.4f, AngularV);
	}

#endif

	TargetVelocity[BODY] = 0;
	TargetAngularV = 0;
	Acceleration = 0;
	AngularAcceleration = 0;
	TotalPulse[LEFT] = 0;
	TotalPulse[RIGHT] = 0;
	TotalPulse[BODY] = 0;

	//両壁の値を取得。それぞれの値と差分を制御目標に反映。
	IMU_Calib();	//これにHAL_Delayがあることで割り込みがずれることがあるのではないか。
	//zg_offset = 0;
#if 0
	TargetPhoto[SL] = Photo[SL];
	TargetPhoto[SR] = Photo[SR];
	PhotoDiff = TargetPhoto[SL] - TargetPhoto[SR];
#else


	TargetPhoto[SL] = Photo[SL];//439.600006;//THRESHOLD_SL;
	TargetPhoto[SR] = Photo[SR];//294.299988;//THRESHOLD_SR;
	PhotoDiff = TargetPhoto[SL] - TargetPhoto[SR];

#endif
	PIDReset(L_VELO_PID);
	PIDReset(R_VELO_PID);

	PIDReset(A_VELO_PID);
	PIDReset(L_WALL_PID);
	PIDReset(R_WALL_PID);
	PIDReset(D_WALL_PID);

#endif
}

void InitFastest()
{
	Motor_PWM_Start();
	EncoderStart(); //戻し忘れないように
	EmitterON();
	ADCStart();

	uint8_t imu_check;
	imu_check =IMU_init();

	printf("imu_check 1ならOK: %d\r\n",imu_check);
	//IMU_DMA_Start();
	//CS_RESET;

	//PID制御準備
	//PIDInit();
	PIDChangeFlag(L_VELO_PID, 0);
	PIDChangeFlag(R_VELO_PID, 0);
	PIDChangeFlag(L_WALL_PID, 0);
	PIDChangeFlag(R_WALL_PID, 0);
	PIDChangeFlag(D_WALL_PID, 0);
	//PIDChangeFlag(B_VELO, 0);
	PIDChangeFlag(A_VELO_PID, 0);


	Load_Gain();
	InitPulse( (int*)(&(TIM3->CNT)),  INITIAL_PULSE);
	InitPulse( (int*)(&(TIM4->CNT)),  INITIAL_PULSE);

#if 0
	wall_init();
	//ジャイロの読み取りにかかる時間の計測
//  int16_t data[1000]={0};
//  int i=0, elaps=0;

  HAL_TIM_Base_Start_IT(&htim8);
t = 1;
	wall_set();//現在座標じゃなくて、進行方向から求めた次の座標。
	//計算して
	UpdateWalkMap();

  t = 0;
  HAL_TIM_Base_Stop_IT(&htim8);
  //data[i] = zg;
  while(1)
  {
	  printf("timer8 : %d\r\n",timer8);
//	  t = 1;
//	  //read_gyro_data();
//	  ZGyro = ReadByte();
//	  t = 0;
//	  HAL_TIM_Base_Stop_IT(&htim8);
//	  data[i] = zg;
//	  i++;
//	  zg = 0;
//	  if(i == 1000)
//	  {
//		  t = 0;
//		  HAL_TIM_Base_Stop_IT(&htim8);
//		  break;
//	  }

  }
#endif
	//割り込みを有効化
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim8);


	//ここまででハードの準備はできた。
	//ここからはソフト的な準備

	TargetVelocity[BODY] = 0;
	TargetAngularV = 0;
	Acceleration = 0;
	AngularAcceleration = 0;
	TotalPulse[LEFT] = 0;
	TotalPulse[RIGHT] = 0;
	TotalPulse[BODY] = 0;

	//両壁の値を取得。それぞれの値と差分を制御目標に反映。
	IMU_Calib();	//これにHAL_Delayがあることで割り込みがずれることがあるのではないか。
	//zg_offset = 0;
	TargetPhoto[SL] = Photo[SL];
	TargetPhoto[SR] = Photo[SR];
	PhotoDiff = TargetPhoto[SL] - TargetPhoto[SR];

	PIDReset(L_VELO_PID);
	PIDReset(R_VELO_PID);
	PIDReset(A_VELO_PID);
	PIDReset(L_WALL_PID);
	PIDReset(R_WALL_PID);
	PIDReset(D_WALL_PID);


}
void Debug()
{
	//テストする
//	if (Flash_clear_sector9() )
//		{
//			printf("OK9\r\n");
//		}
//	if (Flash_clear_sector1() )
//		{
//			printf("OK1\r\n");
//		}
//	if (Flash_clear_sector8())
//	{
//		printf("OK8\r\n");
//	}
//	while(1)
//	{
//
//	}
#if 1
	InitExplore();
	InitPosition();
	wall_init();

	TotalPulse[RIGHT] = 0;
	TotalPulse[LEFT] = 0;
	TotalPulse[BODY] = 0;

	PIDChangeFlag(L_VELO_PID, 1);
	PIDChangeFlag(R_VELO_PID, 1);
	printf("パルスチェック: BODY %d, LEFT %d, RIGHT %d\r\n",TotalPulse[BODY],TotalPulse[LEFT],TotalPulse[RIGHT]);
	//PIDChangeFlagStraight(N_WALL_PID);
	PIDChangeFlag(D_WALL_PID, 0);
	PIDChangeFlag(L_WALL_PID, 0);
	PIDChangeFlag(R_WALL_PID, 0);
	PIDChangeFlag(A_VELO_PID, 1);
	ExploreVelocity=0;
	ChangeLED(3);
	//HAL_Delay(500);

	//IT_mode = WRITINGFREE;
	IT_mode = EXPLORE;
//
#if 0
	TargetAngle = 0;
	TargetVelocity[BODY] = 180;
	velodebug_flag = 1;

	while(velodebug_flag == 1) //速度デバッグ
	{

	}
	TargetVelocity[BODY] = 0;
	HAL_Delay(10000);
	while(1)
	{
		for(int i=0; i < 1000; i++)
		{
			printf("%d, %f, %f\r\n",i, velodebugL[i], velodebugR[i]);
		}
	}
#endif
//	HAL_Delay(500);
//	while(1)
//	{
//		printf("zg:%f, double:%lf\r\n",(float)zg, AngularV);//, double:%lf\r\n");
//	}

	//割り込み処理テスト
#if 0
	ExploreVelocity=240;
	t = 0;
	timer1=0;
	timer8=0;

	TargetVelocity[BODY] = ExploreVelocity;
	TIM1 ->CNT = 0;
	TIM8 ->CNT = 0;//これ大事かも
	t = 1;
	while(1)
	{
		if(t == 0)
		{
			printf("1: %d, 8 :%d, ImuAngV:%f, ImuAngle:%f, ZGyro:%f\r\n",timer1, timer8, AngularV, Angle, ZGyro);
		}

	}
#endif
#if 0
	while(1)
	{
		printf("%f, %f\r\n",TargetPhoto[SL], TargetPhoto[SR] );
		HAL_Delay(2000);
	}
#endif

#if 0 //前壁補正
	Pos.Act = compensate;

	PIDChangeFlag(F_WALL_PID, 1);
	PIDChangeFlag(FD_WALL_PID, 1);
	while(1)
	{
		printf("%f, %f, %f, %f\r\n", Photo[FL], Photo[FR], Photo[FL] - Photo[FR],Photo[FL] + Photo[FR] );
	}
	PIDChangeFlag(F_WALL_PID, 0);
#endif
#if 1 //直進テスト
	ExploreVelocity = 135;
	Pos.Dir = front;
	Accel(61.75,ExploreVelocity);
	for(int i=0; i < 1; i++)
	{
		Pos.Dir = front;
		GoStraight(90, ExploreVelocity, AddVelocity);
		//Pos.Dir = right;
		SlalomRight();

	}
	Pos.Dir = front;
	//Decel(45,0);
	TargetVelocity[BODY] = 0;
	HAL_Delay(1000);

#endif


#if 0 //旋回テスト

	//n回分の角度を取得。
	float theta_log[30];//, angv_log[2000];
	float target_angle_log[30];
	ExploreVelocity=0;
	for(int i=0; i < 30; i+=3)//Photo[FR] < 250)
	{
		ChangeLED(7);
		Pos.Car = north;
		Pos.Dir = back;
		theta_log[i] = Angle;
		target_angle_log[i] = TargetAngle;

		Rotate(90,M_PI*2);
		theta_log[i+1] = Angle;
		target_angle_log[i+1] = TargetAngle;

		HAL_Delay(100);
		theta_log[i+2] = Angle;
		target_angle_log[i+2] = TargetAngle;

		ChangeLED(0);
		Pos.Car = north;
		Pos.Dir = back;
		Rotate(90,-M_PI*2);
		HAL_Delay(100);
		//theta_log[i] = Angle;
	}
	while(1)
			{
				for(int i=0; i < 30; i++)
				{
					printf("%d, %f, %f\r\n",i,theta_log[i], target_angle_log[i]);
				}
			}
#endif

#if 1 //壁制御テスト

	//Uターン
		//袋小路の中央の一区画半手前(90+45mm)からスタートさせる
		//加速で45mm
		//実際何ミリの距離を使えるのか
	//Uターン

	//制御方法がおかしいせいでゆらゆらしているのだと思う
#endif

#if 1 //壁切れテスト
	//
#endif

#else

	EmitterON();
	ADCStart();
	HAL_TIM_Base_Start_IT(&htim8);
	while(1)
	{
		printf("SL:%f, SR:%f, FL:%f, FR:%f\r\n",Photo[SL],Photo[SR],Photo[FL],Photo[FR]);
	}
#endif
}
void ParameterSetting()
{
	Load_Gain();
	Change_Gain();

}

void GainTestLWall()
{
	IT_mode = EXPLORE;
	InitExplore();
	InitPosition();
	wall_init();
	TotalPulse[RIGHT] = 0;
	TotalPulse[LEFT] = 0;
	TotalPulse[BODY] = 0;

	PIDChangeFlag(L_VELO_PID, 1);
	PIDChangeFlag(R_VELO_PID, 1);
	//PIDChangeFlagStraight(L_WALL_PID);
	PIDChangeFlag(D_WALL_PID, 0);
	PIDChangeFlag(L_WALL_PID, 1);
	PIDChangeFlag(R_WALL_PID, 0);
	//PIDSetGain(D_WALL_PID, 10, 0, 0);
	ExploreVelocity=0;
	ChangeLED(4);
	while(1)
	{
		TargetVelocity[BODY] = 300;
	}
}
void GainTestRWall()
{
	IT_mode = EXPLORE;
	InitExplore();
	InitPosition();
	wall_init();
	TotalPulse[RIGHT] = 0;
	TotalPulse[LEFT] = 0;
	TotalPulse[BODY] = 0;

	PIDChangeFlag(L_VELO_PID, 1);
	PIDChangeFlag(R_VELO_PID, 1);
	//PIDChangeFlagStraight(R_WALL_PID);
	PIDChangeFlag(D_WALL_PID, 0);
	PIDChangeFlag(L_WALL_PID, 0);
	PIDChangeFlag(R_WALL_PID, 1);
	//PIDSetGain(D_WALL_PID, 10, 0, 0);
	ExploreVelocity=0;
	ChangeLED(1);
	while(1)
	{
		TargetVelocity[BODY] = 0;

	}
}
void GainTestDWall()
{
	IT_mode = EXPLORE;
	InitExplore();
	InitPosition();
	wall_init();
	TotalPulse[RIGHT] = 0;
	TotalPulse[LEFT] = 0;
	TotalPulse[BODY] = 0;

	PIDChangeFlag(L_VELO_PID, 1);
	PIDChangeFlag(R_VELO_PID, 1);
	//PIDChangeFlagStraight(D_WALL_PID);
	PIDChangeFlag(D_WALL_PID, 1);
	PIDChangeFlag(L_WALL_PID, 0);
	PIDChangeFlag(R_WALL_PID, 0);
	//PIDSetGain(D_WALL_PID, 10, 0, 0);
	ExploreVelocity=0;
	ChangeLED(2);
	while(1)
	{
		TargetVelocity[BODY] = 0;
		printf("前左: %f,前右: %f,横左: %f,横右: %f\r\n",Photo[FL],Photo[FR],Photo[SL],Photo[SR]);
	}
}

void GainTestAVelo()
{
	IT_mode = EXPLORE;
	InitExplore();
	InitPosition();
	wall_init();
	TotalPulse[RIGHT] = 0;
	TotalPulse[LEFT] = 0;
	TotalPulse[BODY] = 0;

	PIDChangeFlag(L_VELO_PID, 1);
	PIDChangeFlag(R_VELO_PID, 1);
	//PIDChangeFlagStraight(A_VELO_PID);
	PIDChangeFlag(A_VELO_PID, 1);
	PIDChangeFlag(D_WALL_PID, 0);
	PIDChangeFlag(L_WALL_PID, 0);
	PIDChangeFlag(R_WALL_PID, 0);
	//PIDSetGain(D_WALL_PID, 10, 0, 0);
	ExploreVelocity=0;
	ChangeLED(5);
	while(1)
	{
		TargetVelocity[BODY] = 0;
		//printf("%f, %f\r\n", AngularV, Angle);

	}
}
void WritingFree()
{
	IT_mode = WRITINGFREE;

	InitExplore();

	printf("3\r\n");

	InitPosition();

	wall_init();
	printf("4\r\n");

	TotalPulse[RIGHT] = 0;
	TotalPulse[LEFT] = 0;
	TotalPulse[BODY] = 0;

	PIDChangeFlag(L_VELO_PID, 1);
	PIDChangeFlag(R_VELO_PID, 1);

	//PIDChangeFlagStraight(N_WALL_PID);
	PIDChangeFlag(D_WALL_PID, 0);
	PIDChangeFlag(L_WALL_PID, 0);
	PIDChangeFlag(R_WALL_PID, 0);
	//PIDSetGain(D_WALL_PID, 10, 0, 0);
	ExploreVelocity=0;
	ChangeLED(7);
#if 0


#else
	while(1)
	{
//		ExploreVelocity=300;
//		GoStraight(9000, ExploreVelocity, 0);
		TargetVelocity[BODY] = 0;


		printf("%f, %f, %f, %f, %f\r\n",ZGyro, Photo[FL],Photo[FR],Photo[FL]+Photo[FR],(Photo[FL]+Photo[FR])/2);//壁センサ前のチェック。

	}
#endif
	Accel(61.5, ExploreVelocity);
	SelectAction('S');
	SelectAction('S');
	Decel(35, 0);

while(1)
{
	//printf("zg : %d, %lf, %f\r\n",zg,(double)zg,(float)zg);	//zgは右回転が負。どの型でもおかしい値は出なかった。
	//printf("AngularV : %f, Angle : %f\r\n",AngularV, Angle);	//モータに出力する際は角速度を負に指定すると左回転。
	//printf("%f, %f, %f\r\n", Photo[FL],Photo[FR],Photo[FL]+Photo[FR]);

}
while(1)
{
	printf("オフセット:%lf, double角速度:%lf, double角度:%lf, float角速度:%f, float角度:%f, 壁センサ値:%f, %f, %f, %f\r\n",zg_offset,ImuAngV, ImuAngle, AngularV, Angle,Photo[SL], Photo[SR], Photo[FL], Photo[FR]);
}
//	while(1){
//		printf("%f,%f,%f,%f\r\n",Photo[SL],Photo[SR],Photo[FL],Photo[FR]);	//SRとFRが等しくなろうとしている
//	}

	//printf("VelocityLeftOut, VelocityRightOut : %d,%d\r\n", VelocityLeftOut, VelocityRightOut);	//ここで変な値が入っている→原因はモード選択用にエンコーダを回したパルスの初期化をしていなかったこと
	//GoStraight( TRUE, 300);
#if 1
	//壁制御と速度制御の相性が悪い
	//打ち消しあわないかつ操作しやすい制御にする。→壁の左右差から角度差を計算して角速度制御させる。
	//角度がθのとき、壁左右値がいくつであるか、という関数を同定し、外部入力から左右値を取得し角度を得る。
	//壁補正は入れるタイミングを決めるのが面倒なので最初はあてにしない。
	//IMUで角速度を入れて、そっちで角度算出するほうを頑張るほうが望みがある。

	Accel(61.5, ExploreVelocity);

	printf("VelocityLeftOut, VelocityRightOut : %d,%d\r\n", VelocityLeftOut, VelocityRightOut);	//微妙に出力値が残る。
#else

	Rotate( 180 , -5);

	HAL_Delay(1000);

	Rotate( 180 , 5);
	//Rotate( 90 , 1*M_PI);
#endif

	while(1)
	{
		TargetAngularV = 0;
	}


	while(1)
	{

		//printf("L_motor, R_motor : %d, %d\r\n",L_motor, R_motor);
		//printf("TargetVelocity[LEFT], TargetVelocity[RIGHT], CurrentVelocity[LEFT], CurrentVelocity[RIGHT] : %f, %f, %f, %f\r\n",TargetVelocity[LEFT], TargetVelocity[RIGHT],  CurrentVelocity[LEFT], CurrentVelocity[RIGHT]);
		//printf("e, ei, ed, elast, out, KP : %f, %f, %f, %f, %d, %f\r\n",pid[LEFT].e, pid[LEFT].ei, pid[LEFT].ed, pid[LEFT].elast, pid[LEFT].out, pid[LEFT].KP);
		//printf("VelocityLeftOut, TargetVelocity[LEFT], CurrentVelocity[LEFT] : %d, %f, %f\r\n", VelocityLeftOut, TargetVelocity[LEFT], CurrentVelocity[LEFT]);
	}
	//探索の場合は迷路とステータスの準備
}

void FastestRun()
{
	IT_mode = EXPLORE;
	//IT_mode = WRITINGFREE;
	//諸々の初期化
	HAL_Delay(100);
	int8_t mode=1;
	  ModeSelect( 1, 2, &mode);
	  Signal( mode );

		HAL_Delay(100);
		  int8_t mode2=1;
		  ModeSelect( 1, 4, &mode2);
		  Signal( mode2 );

		  PhotoSwitch();
	InitFastest();
	InitPosition();


	wall_init();

	TotalPulse[RIGHT] = 0;
	TotalPulse[LEFT] = 0;
	TotalPulse[BODY] = 0;

	PIDChangeFlag(L_VELO_PID, 1);
	PIDChangeFlag(R_VELO_PID, 1);
	printf("パルスチェック: BODY %d, LEFT %d, RIGHT %d\r\n",TotalPulse[BODY],TotalPulse[LEFT],TotalPulse[RIGHT]);
	//PIDChangeFlagStraight(N_WALL_PID);
	PIDChangeFlag(D_WALL_PID, 0);
	PIDChangeFlag(L_WALL_PID, 0);
	PIDChangeFlag(R_WALL_PID, 0);
	//PIDSetGain(D_WALL_PID, 10, 0, 0);

	char turn_mode;
	if(mode == 1)
	{
		ExploreVelocity = 400;
		turn_mode = 'T';
	}
	else if(mode == 2)
	{
		turn_mode = 'S';
	}

	switch(mode2)
	{
	case 1:
		ExploreVelocity=90;
		//未
		Sla.Pre = 9;
		Sla.Fol = 13;
		Sla.Alpha = 0.014;
		Sla.Theta1 = 30;
		Sla.Theta2 = 60;
		Sla.Theta3 = 90;
		break;
	case 2:
		//完
		ExploreVelocity=135;
		Sla.Pre = 5;
		Sla.Fol = 5;
		Sla.Alpha = 0.0273;
		Sla.Theta1 = 30;
		Sla.Theta2 = 60;
		Sla.Theta3 = 90;
		break;
	case 3:
		//未
//		ExploreVelocity=180;
//		Sla.Pre = 5;
//		Sla.Fol = 10;
//		Sla.Alpha = 0.04478;
//		Sla.Theta1 = 30;
//		Sla.Theta2 = 60;
//		Sla.Theta3 = 90;
		ExploreVelocity=180;
		Sla.Pre = 2;
		Sla.Fol = 3.5;
		Sla.Alpha = 0.04;
		Sla.Theta1 = 30;
		Sla.Theta2 = 60;
		Sla.Theta3 = 90;
		break;
	case 4:
		ExploreVelocity=300;
		Sla.Pre = 2;
		Sla.Fol = 19;
		Sla.Alpha = 0.13;
		Sla.Theta1 = 30;
		Sla.Theta2 = 60;
		Sla.Theta3 = 90;
		break;

	}

	ChangeLED(4);


	//マップデータの取得。flashから壁データを取得。
	flash_copy_to_ram();
	printf("あ\r\n");
	//最短経路導出(今回は省けそう。)

	//走る
	fast_run( X_GOAL_LESSER, Y_GOAL_LESSER,X_GOAL_LARGER,Y_GOAL_LARGER, turn_mode);

	//ゴールしたら減速して、停止。
	Decel(45,0);
	//終了合図
	Signal(7);

	while(1)
	{
		HAL_Delay(10*1000);
		printf("ログ出力\r\n");
	}
}
void Explore()
{
	IT_mode = EXPLORE;
	//IT_mode = WRITINGFREE;
	//7で探索へ、0~6でデータ操作。マップを消す、マップをRAMに移す、マップを初期化する。
	//一回目で失敗していたら、flash消してram初期化
	//一回目で成功したら、flashをramに移す

	HAL_Delay(100);
	int8_t mode=1;
	ModeSelect( 1, 2, &mode);
	Signal( mode );
	HAL_Delay(100);

	int8_t mode2=1;
	ModeSelect( 1, 4, &mode2);
	Signal( mode2 );
	PhotoSwitch();
	InitExplore();
	InitPosition();
	wall_init();

	TotalPulse[RIGHT] = 0;
	TotalPulse[LEFT] = 0;
	TotalPulse[BODY] = 0;

	PIDChangeFlag(L_VELO_PID, 1);
	PIDChangeFlag(R_VELO_PID, 1);

	//PIDChangeFlagStraight(N_WALL_PID);
	PIDChangeFlag(D_WALL_PID, 0);
	PIDChangeFlag(L_WALL_PID, 0);
	PIDChangeFlag(R_WALL_PID, 0);
	//PIDSetGain(D_WALL_PID, 10, 0, 0);

	ChangeLED(2);

	//スラロームか、一区画ずつかを選ぶ。
	char turn_mode;
	if(mode == 1)
	{
		turn_mode = 'T';
		ExploreVelocity=300;
	}
	else if(mode == 2)
	{
		turn_mode = 'S';
	}

	switch(mode2)
	{
	case 1:
		ExploreVelocity=90;
		//未
		Sla.Pre = 9;
		Sla.Fol = 20;
		Sla.Alpha = 0.014;
		Sla.Theta1 = 30;
		Sla.Theta2 = 60;
		Sla.Theta3 = 90;

//		ExploreVelocity=180;//*40/1000
//		Sla.Pre = 5;
//		Sla.Fol = 12;
//		Sla.Alalpha = 0.0007;
//		Sla.Theta1 = 30;
//		Sla.Theta2 = 60;
//		Sla.Theta3 = 90;
		break;
	case 2:
		//完
//		ExploreVelocity=135;//*40/1000
//		Sla.Pre = 5;
//		Sla.Fol = 10;
//		Sla.Alpha = 0.0273;
//		Sla.Theta1 = 30;
//		Sla.Theta2 = 60;
//		Sla.Theta3 = 90;



		ExploreVelocity=180;
		Sla.Pre = 2;
		Sla.Fol = 16.5;
		Sla.Alpha = 0.043;
		Sla.Theta1 = 30;
		Sla.Theta2 = 60;
		Sla.Theta3 = 90;
		break;
	case 3:
		ExploreVelocity=240;
		Sla.Pre = 2;
		Sla.Fol = 16;
		Sla.Alpha = 0.078;
		Sla.Theta1 = 30;
		Sla.Theta2 = 60;
		Sla.Theta3 = 90;
		break;
	case 4:
		ExploreVelocity=300;
		Sla.Pre = 2;
		Sla.Fol = 19;
		Sla.Alpha = 0.13;
		Sla.Theta1 = 30;
		Sla.Theta2 = 60;
		Sla.Theta3 = 90;
		//		//未

		break;

	}
	SearchOrFast = 0;
	Pos.Dir = front;
	Pos.Car = north;
	Pos.NextX = Pos.X;
	Pos.NextY = Pos.Y + 1;
	Pos.NextCar = north;
	dbc = 1;
	Accel(61.5, ExploreVelocity);
 	Pos.X = Pos.NextX;
    Pos.Y = Pos.NextY;
	Pos.Car = Pos.NextCar;

	while(  !( (X_GOAL_LESSER <= Pos.X) && (Pos.X <= X_GOAL_LARGER) ) ||  !( ( Y_GOAL_LESSER <= Pos.Y) && (Pos.Y <= Y_GOAL_LARGER) )  ) //&&  (1/*ゴール座標の壁をすべて知っているフラグが0)*/ //ゴール区画内に入っていてかつゴールの区画をすべて知っていれば。
	{

		ChangeLED(Pos.Car);
		KyushinJudge( turn_mode );
#if 0
		static int cc =0;
		cc ++;
		if(cc == 1)
		{
			break;
		}
#else
		//break;
#endif
	}
//	while(1)
//	{
//		TargetVelocity[BODY] = 0;
//	}
	Decel(45, 0);
//	HAL_Delay(10000);
//	while(1)
//	{
//		for (int s=0; s < 8000; s ++)
//		{
//			printf("%d,%f, %f\r\n",s,debugVL[s],debugVR[s]);
//		}
//		HAL_Delay(10000);
//	}

//		printf("total L: %d, total R: %d\r\n",TotalPulse[LEFT],TotalPulse[RIGHT]);
//		HAL_Delay(1000);


	//flashのクリア。
	Flash_clear_sector1();
	//マップ書き込み
	flash_store_init();

	//完了の合図
	Signal(7);

	//flashに保存
	while(1)
	{
		wall_ram_print();
	}
}

void FullyAutonomous()
{
	//五回の走行全てを完全自律で。




}
void CommandAccel(float add_distance,float start_speed,float end_speed)
{
	Command ac;
	float additional_speed = end_speed - start_speed;
	ac.Acceleration = T1*additional_speed*additional_speed / (2*add_distance);
	ac.Mileage = 2*add_distance;
	ac.AngAccel = 0;
	ac.FinishPhase = 0;
	ac.WallDetect_Start = ac.Mileage*0.80;
	setQueue(&cq,ac);
}
void CommandDecel(float dec_distance,float start_speed, float end_speed)
{
	Command dc;
	float down_speed = start_speed - end_speed;
	dc.Acceleration = -1 * (T1*down_speed*down_speed / (2*dec_distance) );
	dc.Mileage = 2*dec_distance;
	dc.AngAccel = 0;
	dc.FinishPhase = 0;
	dc.WallCorrectPos_Start = 0;
	dc.WallCorrectPos_End = (dc.Mileage*0.65);

	setQueue(&cq,dc);
}
void CQ_test()
{
	IT_mode = CQ;
	PhotoSwitch();
	KeepMileage[BODY] = 0;
	TotalMileage[RIGHT] = 0;
	TotalMileage[LEFT] = 0;
	TotalMileage[BODY] = 0;
	InitExplore();
	TotalPulse[RIGHT] = 0;
	TotalPulse[LEFT] = 0;
	TotalPulse[BODY] = 0;

	PIDChangeFlag(L_VELO_PID, 1);
	PIDChangeFlag(R_VELO_PID, 1);

	PIDChangeFlag(D_WALL_PID, 0);
	PIDChangeFlag(L_WALL_PID, 0);
	PIDChangeFlag(R_WALL_PID, 0);

	ChangeLED(1);
	initQueue(&cq);
	CommandAccel(45, 0,90);

	CommandDecel(45, 90,0);

	while(1)
	{
//		if(TotalMileage[BODY] >= 90)
//		{
//			break;
//		}
		//printf("%f,%f\r\n",KeepMileage[BODY] + com.Mileage, TotalMileage[BODY]);

	}
	//printf("%f,%f\r\n",KeepMileage[BODY] + com.Mileage, TotalMileage[BODY]);


}
