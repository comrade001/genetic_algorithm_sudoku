/*Algoritmo Genetico
Se busca resolver un sudoku.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

/*Definicion de la estructura INDIVIDUO*/
typedef struct  {
    unsigned char* cromosoma;   //Genotipo
    float fit;					//Fitness
}INDIVIDUO;

typedef struct{
	unsigned int idGbest;		//Id Best Global
	unsigned int* BitsPorGen;	//No. Bits por Gen
	unsigned char* elite;		//Cromosoma Ind Elite
	float e_fit;				//Fitness Ind Elite
	INDIVIDUO* pInd;
}POBLACION;

/*Variables globales*/
const unsigned int Numero_de_Genes=9;
const unsigned int Numero_de_Individuos=5;
unsigned int MaximoIteraciones=100+1;
const float p_cross=0.8;
const float p_muta=0.12;
unsigned int Genes[9]={[0 ... 8]=9};

/*Prototipo de funciones*/
POBLACION* CrearPoblacion(const unsigned int Numero_de_Individuos);
void InicializarPoblacion(POBLACION *pPob);
void EvaluarPoblacion(POBLACION *pPob);
void EliminarPoblacion(POBLACION *pPob, const unsigned int Numero_de_Individuos);
int SeleccionarPoblacion(POBLACION *pPob);
void CruzarPoblacion(POBLACION *pPob, int padre, int madre);
void MutarPoblacion(POBLACION *pPob, int padre, int madre);
void MostrarPoblacion(POBLACION *pPob);
void InicializarMejores(POBLACION *pPob);
void ActualizarMejores(POBLACION *pPob);
unsigned int LongitudCromosoma(POBLACION *pPob);

int main()
{
	POBLACION *pPob;
	unsigned int padre, madre, It=1, count;
	float worst;
	/*Semilla aleatoria*/
	srand(time(NULL));

	/*Crear memoria para la poblacion*/
	pPob=CrearPoblacion(Numero_de_Individuos);
	/*Inicializacion de los individuos*/
	InicializarPoblacion(pPob);
	/*Evaluacion de la funcion Fitness*/
	EvaluarPoblacion(pPob);
	/*Inicializacion de los mejores individuos*/
	InicializarMejores(pPob);

	/*Se incializa el Ind Elite*/
	pPob->elite=pPob->pInd[pPob->idGbest].cromosoma;
	pPob->e_fit=pPob->pInd[pPob->idGbest].fit;

	printf("\n                              \n");
	printf("Generacion 0: Best=%i\n", pPob->idGbest);
	printf("                                \n");
	/*Mostrar poblacion*/
	MostrarPoblacion(pPob);

	while(pPob->e_fit>100.0)	
	{
		if(pPob->e_fit>pPob->pInd[pPob->idGbest].fit)
			{
				/*Actualizacion del Ind Elite*/
				pPob->elite=pPob->pInd[pPob->idGbest].cromosoma;
				pPob->e_fit=pPob->pInd[pPob->idGbest].fit;
				printf("\n%f\n",pPob->pInd[pPob->idGbest].fit); 
			}

		/*Seleccion del padre y la madre*/
	  	padre=SeleccionarPoblacion(pPob);
	  	madre=SeleccionarPoblacion(pPob);
		
		/*Proceso de gestacion de los indiviudos*/
		CruzarPoblacion(pPob, padre, madre);
		MutarPoblacion(pPob, padre, madre);
		
		/*Evaluacion de la funcion Fitness*/
		EvaluarPoblacion(pPob);
		count=0;
		worst=pPob->pInd[0].fit;

		/*Se localiza al peor individuos*/
		for(unsigned int i=0; i<Numero_de_Individuos; i++)
		{
			if(pPob->pInd[i].fit>worst)
			{
				worst=pPob->pInd[i].fit;
				count=i;
			}
		}
		/*Se sustituye al peor individuo con el Elite*/
		pPob->pInd[count].cromosoma=pPob->elite;
		pPob->pInd[count].fit=pPob->e_fit;

		/*Actualizacion del mejor individuo*/
		ActualizarMejores(pPob);

		printf("\n                              \n");
		printf("Generacion %i: Best=%i\n", It, pPob->idGbest);
		printf("                                \n");
		/*Monstrar poblacion*/
	  	MostrarPoblacion(pPob);

		It++;
	}
	
	/*Liberar espacio en memoria para la poblacion,
	 * asi como todos sus individuos*/
	EliminarPoblacion(pPob, Numero_de_Individuos);

	return 0;
}

unsigned int LongitudCromosoma(POBLACION *pPob)
{
	unsigned int i, crom_len=0;
	/*Longitud del cromosoma*/
	for(i=0; i<Numero_de_Genes; i++)
		crom_len+=pPob->BitsPorGen[i];

	return crom_len;
}

void ActualizarMejores(POBLACION *pPob)
{
	unsigned int i;
	double Best;
	Best=pPob->pInd[pPob->idGbest].fit;
	for(i=0; i<Numero_de_Individuos; i++)
	{
		if(pPob->pInd[i].fit < Best)
		{
			pPob->idGbest=i;
			Best=pPob->pInd[i].fit;
		}
	}
}

void InicializarMejores(POBLACION *pPob)
{
	unsigned int k;
	double Best;
	pPob->idGbest=0;
	Best=pPob->pInd[0].fit;
	for(k=0; k<Numero_de_Individuos; k++)
	{
		if(pPob->pInd[k].fit < Best)
		{
			pPob->idGbest=k;
			Best=pPob->pInd[k].fit;
		}
	}
}

void MostrarPoblacion(POBLACION *pPob)
{
	unsigned int i, j, k;

	/*Por cada individuo*/
	for(k=0; k<Numero_de_Individuos; k++)
	{
		printf("Ind[%u]\n", k);
		for(i=0; i<3; i++)
		{
			for(j=0; j<3; j++)
				printf(" %c ", pPob->pInd[k].cromosoma[i*9+j]);
			printf("|");
		}
		printf("\n");
		for(i=0; i<3; i++)
		{
			for(j=3; j<6; j++)
				printf(" %c ", pPob->pInd[k].cromosoma[i*9+j]);
			printf("|");
		}
		printf("\n");
		for(i=0; i<3; i++)
		{
			for(j=6; j<9; j++)
				printf(" %c ", pPob->pInd[k].cromosoma[i*9+j]);
			printf("|");
		}
		printf("\n");
		printf("------------------------------\n");

		for(i=3; i<6; i++)
		{
			for(j=0; j<3; j++)
				printf(" %c ", pPob->pInd[k].cromosoma[i*9+j]);
			printf("|");
		}
		printf("\n");
		for(i=3; i<6; i++)
		{
			for(j=3; j<6; j++)
				printf(" %c ", pPob->pInd[k].cromosoma[i*9+j]);
			printf("|");
		}
		printf("\n");
		for(i=3; i<6; i++)
		{
			for(j=6; j<9; j++)
				printf(" %c ", pPob->pInd[k].cromosoma[i*9+j]);
			printf("|");
		}
		printf("\n");
		printf("------------------------------\n");

		for(i=6; i<9; i++)
		{
			for(j=0; j<3; j++)
				printf(" %c ", pPob->pInd[k].cromosoma[i*9+j]);
			printf("|");
		}
		printf("\n");
		for(i=6; i<9; i++)
		{
			for(j=3; j<6; j++)
				printf(" %c ", pPob->pInd[k].cromosoma[i*9+j]);
			printf("|");
		}
		printf("\n");
		for(i=6; i<9; i++)
		{
			for(j=6; j<9; j++)
				printf(" %c ", pPob->pInd[k].cromosoma[i*9+j]);
			printf("|");
		}
		printf("\nFitness[%u]: %f\n\n", k, pPob->pInd[k].fit);
	}
}

void MutarPoblacion(POBLACION *pPob, int padre, int madre)
{
	unsigned int i, j, count, aux1;
	unsigned int rand1, rand2, rand3;
	float random;
	random=rand()/(float)RAND_MAX;
	if(random < p_muta)
	{
		/*Por cada sub-bloque*/
		for(i=0, count=0; i<9; i++, count++)
		{
			random=rand()/(float)RAND_MAX;	
			if(random>=0.0&&random<0.2)
			{
				do
				{
					rand1=rand()%9;
					rand2=rand()%9;
				}while(rand1==rand2);

				/*1-swap*/
				aux1=(pPob->pInd[padre].cromosoma[rand1+count*9]-'0');
				pPob->pInd[padre].cromosoma[rand1+count*9]=pPob->pInd[padre].cromosoma[rand2+count*9];
				pPob->pInd[padre].cromosoma[rand2+count*9]=aux1+'0';

				aux1=(pPob->pInd[madre].cromosoma[rand1+count*9]-'0');
				pPob->pInd[madre].cromosoma[rand1+count*9]=pPob->pInd[madre].cromosoma[rand2+count*9];
				pPob->pInd[madre].cromosoma[rand2+count*9]=aux1+'0';
			}
			if(random>=0.2&&random<0.5)
			{
				do
				{
					rand1=rand()%9;
					rand2=rand()%9;
					rand3=rand()%9;
				}while(rand1==rand2||rand2==rand3||rand1==rand3);
				
				/*3-swap*/
				aux1=pPob->pInd[padre].cromosoma[rand1+count*9]-'0';
				pPob->pInd[padre].cromosoma[rand1+count*9]=pPob->pInd[padre].cromosoma[rand2+count*9];
				pPob->pInd[padre].cromosoma[rand2+count*9]=pPob->pInd[padre].cromosoma[rand3+count*9];
				pPob->pInd[padre].cromosoma[rand3+count*9]=aux1+'0';

				aux1=pPob->pInd[madre].cromosoma[rand1+count*9]-'0';
				pPob->pInd[madre].cromosoma[rand1+count*9]=pPob->pInd[madre].cromosoma[rand2+count*9];
				pPob->pInd[madre].cromosoma[rand2+count*9]=pPob->pInd[madre].cromosoma[rand3+count*9];
				pPob->pInd[madre].cromosoma[rand3+count*9]=aux1+'0';
			}
			if(random>=0.5&&random<=1.0)
			{
				/*Insercion*/
				aux1=pPob->pInd[padre].cromosoma[count*9]-'0';
				for(j=count*9; j<(count+1)*9; j++)
					pPob->pInd[padre].cromosoma[j]=pPob->pInd[padre].cromosoma[j+1];
				pPob->pInd[padre].cromosoma[((count+1)*9)-1]=aux1+'0';

				aux1=pPob->pInd[madre].cromosoma[count*9]-'0';
				for(j=count*9; j<(count+1)*9; j++)
					pPob->pInd[madre].cromosoma[j]=pPob->pInd[madre].cromosoma[j+1];
				pPob->pInd[madre].cromosoma[((count+1)*9)-1]=aux1+'0';
			}
		}
	}
}

void CruzarPoblacion(POBLACION *pPob, int padre, int madre)
{
	unsigned int crom_len;
	unsigned int i;
	float random;
	
	crom_len=LongitudCromosoma(pPob);
	
	unsigned char aux1[crom_len], aux2[crom_len];
	random=rand()/(float)RAND_MAX;

	if(random<p_cross)
	{
		/*Crossover Uniforme*/
		for(i=0; i<9; i++)
		{
			random=rand()/(float)RAND_MAX;
			if(random<0.5)
				memcpy(&aux1[i*9], &pPob->pInd[padre].cromosoma[i*9], 9*sizeof(unsigned char));
			else
				memcpy(&aux1[i*9], &pPob->pInd[madre].cromosoma[i*9], 9*sizeof(unsigned char));
		}

		for(i=0; i<9; i++)
		{
			random=rand()/(float)RAND_MAX;
			if(random<0.5)
				memcpy(&aux2[i*9], &pPob->pInd[padre].cromosoma[i*9], 9*sizeof(unsigned char));
			else
				memcpy(&aux2[i*9], &pPob->pInd[madre].cromosoma[i*9], 9*sizeof(unsigned char));
		}
		memcpy(pPob->pInd[padre].cromosoma, aux1, (crom_len)*sizeof(unsigned char));	
		memcpy(pPob->pInd[madre].cromosoma, aux2, (crom_len)*sizeof(unsigned char));
	}
}

int SeleccionarPoblacion(POBLACION *pPob)
{
	unsigned int i;
	float f_t=0.0, p_i[Numero_de_Individuos], offset=0.0;
	double max;
	float random=rand()/(float)RAND_MAX;
	int flecha=0;

	/*Localizar best fitness*/
	max=pPob->pInd[0].fit;
	for(i=0; i<Numero_de_Individuos; i++)
	{
		if(pPob->pInd[i].fit>max)
			max=pPob->pInd[i].fit;
	}

	for(i=0; i<Numero_de_Individuos; i++)
		f_t=f_t+(max-pPob->pInd[i].fit);

	for(i=0; i<Numero_de_Individuos; i++)
		p_i[i]=(max-pPob->pInd[i].fit)/f_t;

	/*Metodo de la ruleta*/
	for(i=0; i<Numero_de_Individuos; i++)
	{
		offset+=p_i[i];
		if(random<offset)
		{
			flecha=i;
			return flecha;
		}
	}
	return -1;
}

void EvaluarPoblacion(POBLACION *pPob)
{	
	unsigned int i, j, m, n;
	int sum_f=0, sum_c=0;
	float pro_f=1.0, pro_c=1.0;
	unsigned int f_sum[9]={0}, c_sum[9]={0}, f_pro[9]={0}, c_pro[9]={0};

	/*Evaluacion de la funcion para obtener fitness*/
	
	for(i=0; i<Numero_de_Individuos; i++)
	{
		/*Fila 1, Columna 1*/
		for(m=0; m<3; m++)
			for(n=0; n<3; n++)
			{
				sum_f+=pPob->pInd[i].cromosoma[m*9+n]-'0';
				pro_f*=pPob->pInd[i].cromosoma[m*9+n]-'0';
			}
		for(m=0; m<7; m+=3)
			for(n=0; n<7; n+=3)
			{
				sum_c+=pPob->pInd[i].cromosoma[m*9+n]-'0';
				pro_c*=pPob->pInd[i].cromosoma[m*9+n]-'0';
			}
		if((45-sum_f)<0)
			f_sum[0]=(-1)*(45-sum_f);
		else
			f_sum[0]=45-sum_f;
		if((362880.0-pro_f)<0)	
			f_pro[0]=(-1)*(362880.0-pro_f);
		else
			f_pro[0]=362880.0-pro_f;
		if((45-sum_c)<0)
			c_sum[0]=(-1)*(45-sum_c);
		else
			c_sum[0]=45-sum_c;
		if((362880.0-pro_c)<0)
			c_pro[0]=(-1)*(362880.0-pro_c);
		else
			c_pro[0]=362880.0-pro_c;

		sum_c=sum_f=0;
		pro_c=pro_f=1.0;
		
		/*Fila 2, Columna 2*/
		for(m=0; m<3; m++)
			for(n=3; n<6; n++)
			{
				sum_f+=pPob->pInd[i].cromosoma[m*9+n]-'0';
				pro_f*=pPob->pInd[i].cromosoma[m*9+n]-'0';
			}
		for(m=0; m<7; m+=3)
			for(n=1; n<8; n+=3)
			{
				sum_c+=pPob->pInd[i].cromosoma[m*9+n]-'0';
				pro_c*=pPob->pInd[i].cromosoma[m*9+n]-'0';
			}
		if((45-sum_f)<0)
			f_sum[1]=(-1)*(45-sum_f);
		else
			f_sum[1]=45-sum_f;
		if((362880.0-pro_f)<0)	
			f_pro[1]=(-1)*(362880.0-pro_f);
		else
			f_pro[1]=362880.0-pro_f;
		if((45-sum_c)<0)
			c_sum[1]=(-1)*(45-sum_c);
		else
			c_sum[1]=45-sum_c;
		if((362880.0-pro_c)<0)
			c_pro[1]=(-1)*(362880.0-pro_c);
		else
			c_pro[1]=362880.0-pro_c;

		sum_c=sum_f=0;
		pro_c=pro_f=1.0;

		/*Fila 3, Columna 3*/
		for(m=0; m<3; m++)
			for(n=6; n<9; n++)
			{
				sum_f+=pPob->pInd[i].cromosoma[m*9+n]-'0';
				pro_f*=pPob->pInd[i].cromosoma[m*9+n]-'0';
			}
		for(m=0; m<7; m+=3)
			for(n=2; n<9; n+=3)
			{
				sum_c+=pPob->pInd[i].cromosoma[m*9+n]-'0';
				pro_c*=pPob->pInd[i].cromosoma[m*9+n]-'0';
			}
		if((45-sum_f)<0)
			f_sum[2]=(-1)*(45-sum_f);
		else
			f_sum[2]=45-sum_f;
		if((362880.0-pro_f)<0)	
			f_pro[2]=(-1)*(362880.0-pro_f);
		else
			f_pro[2]=362880.0-pro_f;
		if((45-sum_c)<0)
			c_sum[2]=(-1)*(45-sum_c);
		else
			c_sum[2]=45-sum_c;
		if((362880.0-pro_c)<0)
			c_pro[2]=(-1)*(362880.0-pro_c);
		else
			c_pro[2]=362880.0-pro_c;

		sum_c=sum_f=0;
		pro_c=pro_f=1.0;

		/*Fila 4, Columna 4*/
		for(m=0; m<3; m++)
			for(n=27; n<30; n++)
			{
				sum_f+=pPob->pInd[i].cromosoma[m*9+n]-'0';
				pro_f*=pPob->pInd[i].cromosoma[m*9+n]-'0';
			}
		for(m=0; m<7; m+=3)
			for(n=9; n<16; n+=3)
			{
				sum_c+=pPob->pInd[i].cromosoma[m*9+n]-'0';
				pro_c*=pPob->pInd[i].cromosoma[m*9+n]-'0';
			}
		if((45-sum_f)<0)
			f_sum[3]=(-1)*(45-sum_f);
		else
			f_sum[3]=45-sum_f;
		if((362880.0-pro_f)<0)	
			f_pro[3]=(-1)*(362880.0-pro_f);
		else
			f_pro[3]=362880.0-pro_f;
		if((45-sum_c)<0)
			c_sum[3]=(-1)*(45-sum_c);
		else
			c_sum[3]=45-sum_c;
		if((362880.0-pro_c)<0)
			c_pro[3]=(-1)*(362880.0-pro_c);
		else
			c_pro[3]=362880.0-pro_c;

		sum_c=sum_f=0;
		pro_c=pro_f=1.0;

		/*Fila 5, Columna 5*/
		for(m=0; m<3; m++)
			for(n=30; n<33; n++)
			{
				sum_f+=pPob->pInd[i].cromosoma[m*9+n]-'0';
				pro_f*=pPob->pInd[i].cromosoma[m*9+n]-'0';
			}
		for(m=0; m<7; m+=3)
			for(n=10; n<17; n+=3)
			{
				sum_c+=pPob->pInd[i].cromosoma[m*9+n]-'0';
				pro_c*=pPob->pInd[i].cromosoma[m*9+n]-'0';
			}
		if((45-sum_f)<0)
			f_sum[4]=(-1)*(45-sum_f);
		else
			f_sum[4]=45-sum_f;
		if((362880.0-pro_f)<0)	
			f_pro[4]=(-1)*(362880.0-pro_f);
		else
			f_pro[4]=362880.0-pro_f;
		if((45-sum_c)<0)
			c_sum[4]=(-1)*(45-sum_c);
		else
			c_sum[4]=45-sum_c;
		if((362880.0-pro_c)<0)
			c_pro[4]=(-1)*(362880.0-pro_c);
		else
			c_pro[4]=362880.0-pro_c;

		sum_c=sum_f=0;
		pro_c=pro_f=1.0;

		/*Fila 6, Columna 6*/
		for(m=0; m<3; m++)
			for(n=33; n<36; n++)
			{
				sum_f+=pPob->pInd[i].cromosoma[m*9+n]-'0';
				pro_f*=pPob->pInd[i].cromosoma[m*9+n]-'0';
			}
		for(m=0; m<7; m+=3)
			for(n=11; n<18; n+=3)
			{
				sum_c+=pPob->pInd[i].cromosoma[m*9+n]-'0';
				pro_c*=pPob->pInd[i].cromosoma[m*9+n]-'0';
			}
		if((45-sum_f)<0)
			f_sum[5]=(-1)*(45-sum_f);
		else
			f_sum[5]=45-sum_f;
		if((362880.0-pro_f)<0)	
			f_pro[5]=(-1)*(362880.0-pro_f);
		else
			f_pro[5]=362880.0-pro_f;
		if((45-sum_c)<0)
			c_sum[5]=(-1)*(45-sum_c);
		else
			c_sum[5]=45-sum_c;
		if((362880.0-pro_c)<0)
			c_pro[5]=(-1)*(362880.0-pro_c);
		else
			c_pro[5]=362880.0-pro_c;

		sum_c=sum_f=0;
		pro_c=pro_f=1.0;

		/*Fila 7, Columna 7*/
		for(m=0; m<3; m++)
			for(n=54; n<57; n++)
			{
				sum_f+=pPob->pInd[i].cromosoma[m*9+n]-'0';
				pro_f*=pPob->pInd[i].cromosoma[m*9+n]-'0';
			}
		for(m=0; m<7; m+=3)
			for(n=18; n<25; n+=3)
			{
				sum_c+=pPob->pInd[i].cromosoma[m*9+n]-'0';
				pro_c*=pPob->pInd[i].cromosoma[m*9+n]-'0';
			}
		if((45-sum_f)<0)
			f_sum[6]=(-1)*(45-sum_f);
		else
			f_sum[6]=45-sum_f;
		if((362880.0-pro_f)<0)	
			f_pro[6]=(-1)*(362880.0-pro_f);
		else
			f_pro[6]=362880.0-pro_f;
		if((45-sum_c)<0)
			c_sum[6]=(-1)*(45-sum_c);
		else
			c_sum[6]=45-sum_c;
		if((362880.0-pro_c)<0)
			c_pro[6]=(-1)*(362880.0-pro_c);
		else
			c_pro[6]=362880.0-pro_c;

		sum_c=sum_f=0;
		pro_c=pro_f=1.0;

		/*Fila 8, Columna 8*/
		for(m=0; m<3; m++)
			for(n=57; n<60; n++)
			{
				sum_f+=pPob->pInd[i].cromosoma[m*9+n]-'0';
				pro_f*=pPob->pInd[i].cromosoma[m*9+n]-'0';
			}
		for(m=0; m<7; m+=3)
			for(n=19; n<26; n+=3)
			{
				sum_c+=pPob->pInd[i].cromosoma[m*9+n]-'0';
				pro_c*=pPob->pInd[i].cromosoma[m*9+n]-'0';
			}
		if((45-sum_f)<0)
			f_sum[7]=(-1)*(45-sum_f);
		else
			f_sum[7]=45-sum_f;
		if((362880.0-pro_f)<0)	
			f_pro[7]=(-1)*(362880.0-pro_f);
		else
			f_pro[7]=362880.0-pro_f;
		if((45-sum_c)<0)
			c_sum[7]=(-1)*(45-sum_c);
		else
			c_sum[7]=45-sum_c;
		if((362880.0-pro_c)<0)
			c_pro[7]=(-1)*(362880.0-pro_c);
		else
			c_pro[7]=362880.0-pro_c;

		sum_c=sum_f=0;
		pro_c=pro_f=1.0;

		/*Fila 9, Columna 9*/
		for(m=0; m<3; m++)
			for(n=60; n<63; n++)
			{
				sum_f+=pPob->pInd[i].cromosoma[m*9+n]-'0';
				pro_f*=pPob->pInd[i].cromosoma[m*9+n]-'0';
			}
		for(m=0; m<7; m+=3)
			for(n=20; n<27; n+=3)
			{
				sum_c+=pPob->pInd[i].cromosoma[m*9+n]-'0';
				pro_c*=pPob->pInd[i].cromosoma[m*9+n]-'0';
			}
		if((45-sum_f)<0)
			f_sum[8]=(-1)*(45-sum_f);
		else
			f_sum[8]=45-sum_f;
		if((362880.0-pro_f)<0)	
			f_pro[8]=(-1)*(362880.0-pro_f);
		else
			f_pro[8]=362880.0-pro_f;
		if((45-sum_c)<0)
			c_sum[8]=(-1)*(45-sum_c);
		else
			c_sum[8]=45-sum_c;
		if((362880.0-pro_c)<0)
			c_pro[8]=(-1)*(362880.0-pro_c);
		else
			c_pro[8]=362880.0-pro_c;

		sum_c=sum_f=0;
		pro_c=pro_f=1.0;

		for(j=0; j<9; j++)
		{
			sum_f+=f_sum[j];
			sum_c+=c_sum[j];
			pro_f+=sqrt(f_pro[j]);
			pro_c+=sqrt(c_pro[j]);
		}
		/*Funcion Fitness*/
		pPob->pInd[i].fit=10*(sum_f+sum_c)+pro_f+pro_c;
	}
}

void InicializarPoblacion(POBLACION *pPob)
{
	unsigned int i, j, k, aux, count;
	unsigned int random, numbers[9]={0};

	/*Se llena el arreglo con valores del 1-9
	 * de forma continua*/
	for(i=0; i<9; i++)
		numbers[i]=i+1;

	/*Se mezcla el arreglo recien creado*/
	for(i=0; i<Numero_de_Individuos; i++)
	{
		for(j=0, count=0; j<9; j++, count++)
		{
			for(k=0; k<9; k++)
			{
				aux=numbers[k];
				random=rand()%9;
				numbers[k]=numbers[random];
				numbers[random]=aux;
			}

			for(k=0; k<9; k++)
				pPob->pInd[i].cromosoma[count*9+k]=numbers[k]+'0';
		}
	}
}

POBLACION* CrearPoblacion(const unsigned int Numero_de_Individuos)
{
	POBLACION *pPob;
	unsigned int i, crom_len;
	/*Asignar memoria a la estructura Poblacion*/
	pPob=(POBLACION*)malloc(sizeof(POBLACION));
	if(pPob == NULL)
	{
		printf("Error en memoria\n");
		exit(0);
	}
	/*Asignar memoria a la estructura Individuo*/
	pPob->pInd=(INDIVIDUO*)malloc(sizeof(INDIVIDUO)*Numero_de_Individuos);
	pPob->BitsPorGen=Genes;
	if(pPob->pInd == NULL)
	{
		printf("Error en memoria\n");
		exit(0);
	}

	crom_len=LongitudCromosoma(pPob);
	pPob->elite=(unsigned char*)malloc(sizeof(unsigned char)*crom_len);
	for(i=0; i<Numero_de_Individuos; i++)
		pPob->pInd[i].cromosoma=(unsigned char*)malloc(sizeof(unsigned char)*crom_len);

	return(pPob);
}

void EliminarPoblacion(POBLACION *pPob, const unsigned int Numero_de_Individuos)
{
	unsigned int i;
	/*Liberar memoria para cada individuo*/
	for(i=0; i<Numero_de_Individuos; i++)
	{
		free(pPob->pInd[i].cromosoma);
	}
	/*Liberar memoria de los individuos*/
	free(pPob->pInd);
	free(pPob->elite);
	/*Liberar memoria de la poblacion*/
	free(pPob);
}
