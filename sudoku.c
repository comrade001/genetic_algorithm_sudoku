/*Algoritmo Genetico
Se busca resolver un sudoku.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

//Definicion de la estructura INDIVIDUO
typedef struct  {
    unsigned char* cromosoma;   //genotipo
    long double* valor; //fenotipo (convierte el valor bit a decimal)
    float fit;
	float best_fit;
}INDIVIDUO;

typedef struct{
	unsigned int idGbest;
	unsigned int* BitsPorGen;
	INDIVIDUO* pInd;
}POBLACION;

/*Variables globales*/
const unsigned int Numero_de_Genes=9;
const unsigned int Numero_de_Individuos=10;
//const float LimitInf=1.0;
//const float LimitSup=9.0;
unsigned int MaximoIteraciones=10;
const float pc=0.8; 
const float p_muta=0.12;
unsigned int Genes[9]={[0 ... 8]=9};

/*Prototipo de funciones*/
//POBLACION* CrearPoblacion(const unsigned int Numero_de_Genes, const unsigned int Numero_de_Individuos);
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
	unsigned int padre, madre, It=1;
	/*Se inicializa semilla para generar numeros aleatorios*/
	srand(time(NULL));

	/*Crear memoria para la poblacion*/
	pPob=CrearPoblacion(Numero_de_Individuos);
	InicializarPoblacion(pPob);
	EvaluarPoblacion(pPob);
	InicializarMejores(pPob);

	printf("\n                              \n");
	printf("Generacion 0: Best=%i\n", pPob->idGbest);
	printf("                                \n");
	MostrarPoblacion(pPob);


	while(It<MaximoIteraciones)	
	{

	  	padre=SeleccionarPoblacion(pPob);
	  	madre=SeleccionarPoblacion(pPob);
		
		CruzarPoblacion(pPob, padre, madre);
		MutarPoblacion(pPob, padre, madre);
		EvaluarPoblacion(pPob);
		ActualizarMejores(pPob);
		
		printf("\n                              \n");
		printf("Generacion %i: Best=%i\n", It, pPob->idGbest);
		printf("                                \n");

	  	MostrarPoblacion(pPob);

		It++;
	}
	
	/*Liberar espacio en memoria para la poblacion,
	 * asi como todos sus individuos*/
	//EliminarPoblacion(pPob, Numero_de_Individuos);

	return 0;
}

unsigned int LongitudCromosoma(POBLACION *pPob)
{
	unsigned int i, crom_len=0;
	/*Longitud del cormosoma*/
	for(i=0; i<Numero_de_Genes; i++)
		crom_len+=pPob->BitsPorGen[i];

	return crom_len;
}

void ActualizarMejores(POBLACION *pPob)
{
	unsigned int i;
	float Best;
	Best=pPob->pInd[pPob->idGbest].fit;
	for(i=0; i<Numero_de_Individuos; i++)
	{
		//if(pPob->pInd[i].fit < pPob->pInd[i].best_fit)
		//	pPob->pInd[i].best_fit=pPob->pInd[i].fit;
	
		if(pPob->pInd[i].fit < Best)
		{
			pPob->idGbest=i;
			Best=pPob->pInd[i].fit;
		}
	}

	//for(i=0; i<Numero_de_Individuos; i++)
	//	pPob->pInd[i].best_fit=0;
}

void InicializarMejores(POBLACION *pPob)
{
	unsigned int k;
	float Best;
	pPob->idGbest=0;
	Best=pPob->pInd[0].fit;
	for(k=0; k<Numero_de_Individuos; k++)
	{
		//pPob->pInd[k].best_fit=pPob->pInd[k].fit;
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

	for(i=0; i<Numero_de_Individuos; i++)
	{
		printf("Ind[%u]\n", i);
		printf("-------------------------------\n");
		for(j=0; j<9; j++)
		{	
			for(k=0; k<9; k++)
			{
				if(k==0||k==3||k==6)
					printf("|");
				printf(" %c ", pPob->pInd[i].cromosoma[j*9+k]);
				if(k==8)
					printf("|");

			}
			printf("\n");
			if(j==2||j==5||j==8)
				printf("-------------------------------\n");
		}
		printf("%f\n\n", pPob->pInd[i].fit);
	}
}

void MutarPoblacion(POBLACION *pPob, int padre, int madre)
{
	unsigned int i, l, count=0, m, n, aux;
	float random;
	random=rand()/(float)RAND_MAX;
	if(random<p_muta)
	{
		/*Por cada sub-bloque*/
		for(i=0;i<9;i++)
		{
			random=rand()/(float)RAND_MAX;	
			if(random>0.0&&random<0.5)
			{
				/*Valor entre 0-8*/
				do
				{
					m=rand()%9;
					n=rand()%9;
				}while(m==n);

			}
		}
	}
}

void CruzarPoblacion(POBLACION *pPob, int padre, int madre)
{
	unsigned int crom_len, p_aux;
	unsigned int i;
	float random;
	
	crom_len=LongitudCromosoma(pPob);
	
	unsigned char aux1[crom_len], aux2[crom_len];
	//printf("px=%i\n", px);
	for(i=0; i<crom_len; i++)
	{
		random=rand()/(float)RAND_MAX;
		if(random<0.5)
			aux1[i]=pPob->pInd[padre].cromosoma[i];
		else
			aux1[i]=pPob->pInd[madre].cromosoma[i];
	}

	for(i=0; i<crom_len; i++)
	{
		random=rand()/(float)RAND_MAX;
		if(random<0.5)
			aux2[i]=pPob->pInd[padre].cromosoma[i];
		else
			aux2[i]=pPob->pInd[madre].cromosoma[i];
	}



	//memcpy(aux2, pPob->pInd[padre].cromosoma, px*sizeof(unsigned char));
	//memcpy(aux1, pPob->pInd[madre].cromosoma, px*sizeof(unsigned char));
	//memcpy(&aux1[px], &pPob->pInd[padre].cromosoma[px], ((crom_len)-px)*sizeof(unsigned char));
	//memcpy(&aux2[px], &pPob->pInd[madre].cromosoma[px], ((crom_len)-px)*sizeof(unsigned char));
	
	memcpy(pPob->pInd[padre].cromosoma, aux1, (crom_len)*sizeof(unsigned char));	
	memcpy(pPob->pInd[madre].cromosoma, aux2, (crom_len)*sizeof(unsigned char));
	//printf("%s\t%s\n", aux1, aux2);
	//unsigned int j;
	//for(j=0; j<(crom_len); j++)
	//	printf("%c", pPob->pInd[padre].cromosoma[j]);
	//printf("\t");
	//for(j=0; j<(crom_len); j++)
	//	printf("%c", pPob->pInd[madre].cromosoma[j]);
	//printf("\n");
}

int SeleccionarPoblacion(POBLACION *pPob)
{
	unsigned int i;
	float f_t=0.0, p_i[Numero_de_Individuos], offset=0.0, max;
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
			//break;
			return flecha;
		}
	}
	return -1;
}

void EvaluarPoblacion(POBLACION *pPob)
{	
	unsigned int i, j, k, l=0;
	int sum_f=0, sum_c=0;
	float pro_f=1.0, pro_c=1.0;
	unsigned int f_sum[9]={0}, c_sum[9]={0}, f_pro[9]={0}, c_pro[9]={0};

	/*Evaluacion de la funcion para obtener fitness*/
	
	for(i=0; i<Numero_de_Individuos; i++)
	{
		for(j=0; j<9; j++)
		{
			for(k=0; k<9; k++)
			{
				sum_f+=(pPob->pInd[i].cromosoma[j*9+k]-'0');
				pro_f*=(pPob->pInd[i].cromosoma[j*9+k]-'0');
				sum_c+=(pPob->pInd[i].cromosoma[k*9+j]-'0');
				pro_c*=(pPob->pInd[i].cromosoma[k*9+j]-'0');
			}
				if((45-sum_f)<0)
					f_sum[l]=(-1)*(45-sum_f);
				else
					f_sum[l]=45-sum_f;
				if((362880.0-pro_f)<0)	
					f_pro[l]=(-1)*(362880.0-pro_f);
				else
					f_pro[l]=362880.0-pro_f;
				if((45-sum_c)<0)
					c_sum[l]=(-1)*(45-sum_c);
				else
					c_sum[l]=45-sum_c;
				if((362880.0-pro_c)<0)
					c_pro[l]=(-1)*(362880.0-pro_c);
				else
					c_pro[l]=362880.0-pro_c;
				sum_c=sum_f=0;
				pro_c=pro_f=1.0;
				l++;	
		}
		sum_c=sum_f=l=0;
		pro_c=pro_f=1.0;
		for(j=0; j<9; j++)
		{
			sum_f+=f_sum[j];
			sum_c+=c_sum[j];
			pro_f+=sqrt(f_pro[j]);
			pro_c+=sqrt(c_pro[j]);
		}
		pPob->pInd[i].fit=10*(sum_f+sum_c)+pro_f+pro_c;
		sum_c=sum_f=0;
		pro_c=pro_f=1.0;
	}
}

void InicializarPoblacion(POBLACION *pPob)
{
	unsigned int i, j, crom_len;
	unsigned int random;

	crom_len=LongitudCromosoma(pPob);
	/*Inicializar un valor entre 0 y 1*/	
	for(i=0; i<Numero_de_Individuos; i++)
		for(j=0; j<crom_len; j++)
		{
			random =(rand()%9)+1;
			pPob->pInd[i].cromosoma[j]=random+'0';
		}

	//pPob->pInd[i].fit=0;
}

//POBLACION* CrearPoblacion(const unsigned int Numero_de_Genes, const unsigned int Numero_de_Individuos)
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
	//pPob->BitsPorGen=(unsigned int*)malloc(sizeof(unsigned int)*Numero_de_Genes);
	pPob->BitsPorGen=Genes;
	if(pPob->pInd == NULL)
	{
		printf("Error en memoria\n");
		exit(0);
	}

	crom_len=LongitudCromosoma(pPob);
	for(i=0; i<Numero_de_Individuos; i++)
		{
			pPob->pInd[i].cromosoma=(unsigned char*)malloc(sizeof(unsigned char)*crom_len);
			//pPob->pInd[i].valor=(long double*)malloc(sizeof(long double)*Numero_de_Genes);	
		}

	return(pPob);
}

void EliminarPoblacion(POBLACION *pPob, const unsigned int Numero_de_Individuos)
{
	unsigned int i;
	/*Liberar memoria para cada individuo*/
	for(i=0; i<Numero_de_Individuos; i++)
	{
		free(pPob->pInd[i].cromosoma);
		free(pPob->pInd[i].valor);
	}
	/*Liberar memoria de los individuos*/
	free(pPob->pInd);
	//free(pPob->BitsPorGen);
	/*Liberar memoria de la poblacion*/
	free(pPob);
}
