#include <stdint.h>
#include "thermistor.h"
#include "common.h"

// NTC-10K-II
const float Therm_10K_II[]=
{
	-30.000f,	100.000f,	5.000f,
	176.683f,	130.243f,	96.970f,	//-40~-20
	72.895f,	55.298f,	42.315f,	32.651f,	25.395f,	//-15~5
	19.903f,	15.714f,	12.494f,	10.000f,	8.056f,		//10~30
	6.530f,		5.325f,		4.367f,		3.601f,		2.985f,		//35~55
	2.487f,		2.082f,		1.752f,		1.480f,		1.256f,		//60~80
	1.071f,		0.916f,		0.787f,		0.679f					//85~100
};

// NTC-10K-III
const float Therm_10K_III_F[]=
{
	-30.000f,	100.000f,	5.000f,
	135.233f,	102.890f,	78.930f,	//-40~-20
	61.030f,	47.549f,	37.316f,	29.490f,	23.462f,	//-15~5
	18.787f,	15.136f,	12.268f,	10.000f,	8.197f,		//10~30
	6.754f,		5.594f,		4.656f,		3.893f,		3.271f,		//35~55
	2.760f,		2.339f,		1.990f,		1.700f,		1.458f,		//60~80
	1.255f,		1.084f,		0.940f,		0.817f					//85~100
};

// Honeywell NTC-20K 
const float Therm_20K_HW[]=
{
	-30.0f,		100.0f,		1.0f,
	415.0f,	389.0f,	364.0f,	342.0f,	321.0f,		//-30~-26
	301.0f,	283.0f,	266.0f,	250.0f,	235.0f,		//-25~-21
	221.0f,	208.0f,	196.0f,	184.0f,	174.0f,		//-20~-16
	164.0f,	154.0f,	146.0f,	137.0f,	130.0f,		//-15~-11
	122.0f,	116.0f,	109.0f,	103.0f,	97.6f,		//-10~-6
	92.3f,	87.3f,	82.6f,	78.2f,	74.1f,		//-5~-1
	70.2f,	66.5f,	63.0f,	59.8f,	56.7f,		//0~4
	53.8f,	51.1f,	48.5f,	46.0f,	43.7f,		//5~9
	41.6f,	39.5f,	37.6f,	35.7f,	34.0f,		//10~14
	32.3f,	30.8f,	29.3f,	27.9f,	26.6f,		//15~19
	25.3f,	24.2f,	23.0f,	22.0f,	21.0f,		//20~24
	20.0f,	19.1f,	18.2f,	17.4f,	16.6f,		//25~29
	15.9f,	15.2f,	14.5f,	13.9f,	13.3f,		//30~34
	12.7f,	12.1f,	11.6f,	11.1f,	10.7f,		//35~39
	10.2f,	9.78f,	9.37f,	8.98f,	8.61f,		//40~44
	8.26f,	7.92f,	7.60f,	7.29f,	7.00f,		//45~49
	6.72f,	6.45f,	6.19f,	5.95f,	5.72f,		//50~54
	5.49f,	5.28f,	5.08f,	4.88f,	4.69f,		//55~59
	4.52f,	4.35f,	4.18f,	4.03f,	3.88f,		//60~64
	3.73f,	3.59f,	3.46f,	3.34f,	3.21f,		//65~69
	3.10f,	2.99f,	2.88f,	2.78f,	2.68f,		//70~74
	2.58f,	2.49f,	2.41f,	2.32f,	2.24f,		//75~79
	2.17f,	2.09f,	2.02f,	1.95f,	1.89f,		//80~84
	1.82f,	1.76f,	1.70f,	1.65f,	1.59f,		//85~89
	1.54f,	1.49f,	1.44f,	1.40f,	1.35f,		//90~94
	1.31f,	1.27f,	1.23f,	1.19f,	1.15f,		//95~99
	1.11f										//100
};

const float Therm_MF58_103F3950[]=
{
	-30.000f,	100.000f,	1.000f,		//Min, Max, Resolution
	168.044f,	159.021f,	150.485f,	142.402f,	134.745f,	//-30~-26
	127.490f,	120.614f,	114.099f,	107.928f,	102.085f,	//-25~-21
	96.554f,	91.322f,	86.375f,	81.699f,	77.282f,	//-20~-16
	73.110f,	69.173f,	65.457f,	61.952f,	58.647f,	//-15~-11
	55.530f,	52.591f,	49.821f,	47.210f,	44.748f,	//-10~-6
	42.428f,	40.241f,	38.179f,	36.235f,	34.401f,	//-5~-1
	33.010f,	31.039f,	29.498f,	28.044f,	26.671f,	//0~4
	25.374f,	24.148f,	22.989f,	21.894f,	20.857f,	//5~9
	20.008f,	18.947f,	18.068f,	17.235f,	16.445f,	//10~14
	15.696f,	14.985f,	14.311f,	13.671f,	13.064f,	//15~19
	12.486f,	11.938f,	11.416f,	10.920f,	10.449f,	//20~24
	10.000f,	9.572f,		9.165f,		8.777f,		8.408f,		//25~29
	8.056f,		7.720f,		7.400f,		7.095f,		6.804f,		//30~34
	6.526f,		6.260f,		6.007f,		5.765f,		5.534f,		//35~39
	5.313f,		5.102f,		4.900f,		4.707f,		4.523f,		//40~44
	4.347f,		4.178f,		4.017f,		3.863f,		3.715f,		//45~49
	3.574f,		3.438f,		3.309f,		3.185f,		3.066f,		//50~54
	2.952f,		2.843f,		2.738f,		2.638f,		2.542f,		//55~59
	2.450f,		2.362f,		2.277f,		2.196f,		2.119f,		//60~64
	2.044f,		1.972f,		1.904f,		1.838f,		1.774f,		//65~69
	1.714f,		1.655f,		1.599f,		1.545f,		1.494f,		//70~74
	1.444f,		1.396f,		1.350f,		1.306f,		1.264f,		//75~79
	1.223f,		1.184f,		1.146f,		1.110f,		1.075f,		//80~84
	1.061f,		1.009f,		0.978f,		0.948f,		0.919f,		//85~89
	0.891f,		0.864f,		0.838f,		0.813f,		0.789f,		//90~94
	0.766f,		0.744f,		0.722f,		0.701f,		0.681f,		//95~99
	0.673f														//100
};

float GetTemperature_NTC_Table(float kohm, const float *res_tbl)
{
	float max, min, resolution;
	uint16_t i,points;
	float ret_value= 0.0;
	
	min= res_tbl[0];
	max= res_tbl[1];
	resolution= res_tbl[2];
	
	points= (max - min)/resolution;
	
	if(kohm >= res_tbl[3])
	{
		ret_value= min;
	}
	else
	{
		if(kohm <= res_tbl[points + 3])
		{
			ret_value= max;
		}
		else
		{
			for(i= 0; i< points; i++)
			{
				if(kohm <= res_tbl[i + 3] && kohm > res_tbl[i + 4])
				{
					ret_value= (-1.0f)*(kohm - res_tbl[i + 4])/(res_tbl[i + 3] - res_tbl[i + 4]);
					ret_value*= resolution;
					ret_value+= min + (i + 1)*resolution;
					break;
				}
			}
		}
	}
	
	return ret_value;
}

float GetThermTemp(float kohm, NTC_TYPE_T type, float max, float min)
{
	float ret_value= 0.0f;
	
	switch(type)
	{
		case NTC_10K_B3950:
			ret_value= GetTemperature_NTC_Table(kohm, Therm_MF58_103F3950);  
			break;
		case NTC_20K_HW:
			ret_value= GetTemperature_NTC_Table(kohm, Therm_20K_HW);
			break;
		case NTC_10K_II:
			ret_value= GetTemperature_NTC_Table(kohm, Therm_10K_II);
			break;
		case NTC_10K_III:
			ret_value= GetTemperature_NTC_Table(kohm, Therm_10K_III_F);
			break;
		default:
			break;
	}
	
	if(ret_value > max && max !=0.0f)
	{
		ret_value= max;
	}
	else
	{
		if(ret_value < min && min != 0.0f)
		{
			ret_value= min;
		}
	}
	
	return ret_value;
}

float Temp_F2C(float temp)
{
	return (temp*9.0f/5.0f + 32.0f);
}

float Temp_C2F(float temp)
{
	return ((temp - 32.0f)*5.0f/9.0f);
}
