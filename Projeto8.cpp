#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct CHAVE {
	char *chave;
    int head;
    int bos;
    int pnt_l = -1;
    int pnt_r = -1;
    bool in = false;
} CHAVE;

void Carregar_Pagina_CH(FILE *f,int rrn,CHAVE ch[3]);

FILE *Init_File(char *nm);

int Get_Head(FILE *f, int hd);

int Get_Limit(FILE *f);

void Set_Head(FILE *f,int hd,int vl);

void Insere_id(FILE *f,char *nm,int bos);

void Insere_AB(FILE *f,int rrn,CHAVE ch);

bool Busca_AB(FILE *f,int rrn,CHAVE ch);

void Printa_Pagina(FILE *f,int rrn,CHAVE ch[4]);

void Ordena(CHAVE ch[4]);

int main ()
{
    FILE *ip = Init_File("Index.bin");

    Insere_id(ip,"001003",44);

    Insere_id(ip,"001002",55);

    Insere_id(ip,"001001",55);
    
    Insere_id(ip,"001004",44);

   //CHAVE ch[3];

   //Carregar_Pagina_CH(ip,4,ch);

   //printf("\n%s %s\n",ch[0].chave,ch[1].chave);

   printf("\nFim\n");

}

void Insere_id(FILE *f,char *nm,int bos)
{
    CHAVE ch;

    ch.chave = (char*) malloc(6*sizeof(char));

    strcpy(ch.chave,nm);
    ch.bos = bos;

    //Verificar se a arvore já foi iniciada.
    if(Get_Limit(f) == 0)
    {
        printf("\nInsere_is: Documento vazio.\n");

        Set_Head(f,0,-1);//Cria um head!

    }
    else
    {
        printf("\nInsere_is: Documento iniciado.\n");
    }

    int a = 1;

    if(Get_Head(f,0)==-1)
    {
        rewind(f);   

        Set_Head(f,0,4);

        //Primeira pagina.
        fseek(f,4,0);

        fwrite(&a,sizeof(a),1,f);//Quantidade de chaves

        a = -1;

        fwrite(&a,sizeof(a),1,f);//Ponteiro
        fwrite(nm,strlen(nm),1,f);
        fwrite(&bos,sizeof(bos),1,f);

        fwrite(&a,sizeof(a),1,f);//Ponteiro
        fseek(f,ftell(f)+10,0);

        fwrite(&a,sizeof(a),1,f);//Ponteiro
        fseek(f,ftell(f)+10,0);

        fwrite(&a,sizeof(a),1,f);//Ponteiro

        rewind(f);
    }
    else
    {
        if(Busca_AB(f,Get_Head(f,0),ch)==false)
        {
            Insere_AB(f,Get_Head(f,0),ch);
        }
        else
        {
            printf("\nInsere_id: Esse inidice já foi criado!\n");
        }
        
    }

}

void Set_Head(FILE *f,int hd,int vl)
{
	
	rewind(f);
	
	fseek(f, 4*hd, 0);
	
	fwrite(&vl,sizeof(vl),1,f);
	
	rewind(f);
	
}

void Insere_AB(FILE *f,int rrn,CHAVE ch)
{

    
    CHAVE vch[4];

    Carregar_Pagina_CH(f,rrn,vch);

    int i;

    for(i = 0;i < 4;i++)
    {
        if(i == 3)
        {
            printf("\nInsere_AB: Overflow detectado!\n");

            return;
        }

        if(vch[i].in)
        {
            if(strcmp(vch[i].chave,ch.chave) == 1)
            {
                if(vch[i].pnt_l != -1)
                {

                }
            }

            if(vch[i].pnt_r != -1)
            {

            }
        }
        else
        {
            vch[i].chave = ch.chave;
            vch[i].bos = ch.bos;
            vch[i].in = true;
            break;
        }

    }

    
    Ordena(vch);

    for (i = 0; i < vch[0].head + 1; i++)
    {
        printf("\nA: %s %d\n",vch[i].chave,vch[i].bos);
    }

    ++vch[0].head;

    Printa_Pagina(f,rrn,vch);

}

void Printa_Pagina(FILE *f,int rrn,CHAVE ch[4])
{
    int i;

    fseek(f,rrn,0);

    fwrite(&ch[0].head,sizeof(int),1,f);

    for(i = 0; i < 3; i++)
    {
        fwrite(&ch[i].pnt_l,sizeof(int),1,f);

        if(ch[i].in == true)
        {
            fwrite(ch[i].chave,6,1,f);

            fwrite(&ch[i].bos,sizeof(int),1,f);
        }
        else
        {
            fseek(f,ftell(f)+10,0);
        }
    }

    fwrite(&ch[i].pnt_r,sizeof(int),1,f);
    
}

void Ordena(CHAVE ch[4])
{
    int i, j, min;

    CHAVE ach; 

    int n = ch[0].head;
  
    for (i = 0; i < n; i++) 
    { 

        min = i; 

        for (j = i+1; j < n+1; j++) 
        {
            if((ch[j].in == true) && (ch[min].in == true))
            {
                if (strcmp(ch[j].chave,ch[min].chave) == -1) 
                {
                    min = j; 
                }
            }
            
        }
  
        ach = ch[i];

        ch[i] = ch[min];

        ch[min] = ach;
    }
    ch[0].head = n;
}

bool Busca_AB(FILE *f,int rrn,CHAVE ch)
{
    CHAVE vch[3];

    Carregar_Pagina_CH(f,rrn,vch);
    
    int i;

    for(i = 0;i < vch[0].head;i++)
    {

        if(strcmp(vch[i].chave,ch.chave) == 1)
        {
            if(vch[i].pnt_l != -1)
            {
                Busca_AB(f,vch[i].pnt_l,ch);
            }

            if(vch[i].pnt_r != -1)
            {
                Busca_AB(f,vch[i].pnt_r,ch);
            }
        }

        if(strcmp(vch[i].chave,ch.chave) == 0)
        {
            printf("\nBusca_AB: Achou!\n");
            return true;
        }
    }

    printf("\nBusca_AB: Não achou!\n");

    return false;
}

void Carregar_Pagina_CH(FILE *f,int rrn,CHAVE ch[3])
{
    fseek(f,rrn,0);

    int head;

    fread(&head,sizeof(head),1,f);

    ch[0].head = head;

    int i,j;

    for(i = 0 ; i < head ; i++ )
    {
        ch[i].in = true;

        fread(&ch[i].pnt_l,sizeof(int),1,f);

        ch[i].pnt_r = -1;

        ch[i].chave = (char*) malloc(7*sizeof(char));

        char c[2] = "a";

        for(j = 0;j < 6;j++)
        { 

           ch[i].chave[j] = fgetc(f);   
        }

        ch[i].chave[6] = c[1];

        j = 0;

        fread(&ch[i].bos,sizeof(int),1,f);
    }

    if(head == 3)
    {
        fread(&ch[head-1].pnt_r,sizeof(int),1,f);
    }
}

int Get_Limit(FILE *f)
{
	
	fseek(f,0L, SEEK_END);
	
	int lm = ftell(f);
	
	rewind(f);
	
	return lm;
	
}

int Get_Head(FILE *f, int hd)
{
	
	int h;
	
	rewind(f);
	
	fseek(f, 4*hd, 0);
	
	fread(&h,sizeof(h),1,f);
	
	rewind(f);
	
	return h;
	
}

FILE *Init_File(char *nm)
{
	
	FILE *f;
	
	if((f = fopen( nm , "r+b" )) == NULL)//Tenta abrir o arquivo
	{
		fclose(f);
		
		f = fopen( nm , "w+b" );
		
		printf("\nInit_File: Criacao do arquivo: %s feita com sucesso!\n",nm);
	}
	else
	{	
		printf("\nInit_File: Carregamento do arquivo: %s feita com sucesso!\n",nm);
	}
	
	rewind(f);
	
	return f;//Retorna o arquivo aberto
	
}