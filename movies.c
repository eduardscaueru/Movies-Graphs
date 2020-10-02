#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "movies.h"
#include <string.h>

int duplicate(char **actori, char *nume_actor, int nr_actori)
{
	for(int i = 0; i < nr_actori; i++)
	{
		if(strcmp(nume_actor, actori[i]) == 0) return i;
	}

	return -1;
}

TNode *new_node(char *actor, char *film, int dest)
{
	TNode *new = malloc(sizeof(TNode));
	new->actor = malloc(sizeof(char) * (strlen(actor)+1));
	new->film = malloc(sizeof(char) * (strlen(film))+1);

	memcpy(new->film, film, strlen(film) + 1);
	memcpy(new->actor, actor, strlen(actor) + 1);
	new->v = dest;
	new->next = NULL;

	return new;
}

void add_edge(TGraphL *g, char *nume1, char *nume2, char **actori, char *nume_film)
{
	int v1, v2;
	v1 = duplicate(actori, nume1, g->nr_actori);
	v2 = duplicate(actori, nume2, g->nr_actori);

	TNode *p = g->adl[v1];

	while(p != NULL)
	{
		if(strcmp(p->actor, nume2) == 0) return;
		p = p->next;
	}

	TNode *new1 = new_node(nume2, nume_film, v2);

	new1->next = g->adl[v1];
	g->adl[v1] = new1;
	
	TNode *new2 = new_node(nume1, nume_film, v1);

	new2->next = g->adl[v2];
	g->adl[v2] = new2;
}

TGraphL* creare_graf(char *fin, char c)
{
	FILE *f = fopen(fin, "r");

	TGraphL *g = (TGraphL*)malloc(sizeof(TGraphL));
	int nr, i, x, j, nr_actori = 0, k;
	char *nume_film = malloc(sizeof(char) * 100);
	char *nume_actor = malloc(sizeof(char) * 100);
	char *ignore = malloc(sizeof(char) * 10);
	char **sol = malloc(sizeof(char*) * 500);
	char **actori = malloc(sizeof(char*) * 5000);
	g->actori = malloc(sizeof(char*) * 5000);
	g->filme = malloc(sizeof(char*) * 5000);
	for(i = 0; i < 500; i++)
	{
		sol[i] = (char*)malloc(sizeof(char) * 100);
	}
	for(i = 0; i < 5000; i++)
	{
		actori[i] = (char*)malloc(sizeof(char) * 100);
		g->actori[i] = (char*)malloc(sizeof(char) * 100);
		g->filme[i] = (char*)malloc(sizeof(char) * 100);
	}

	fscanf(f, "%d", &nr);
	fgets(ignore, 10, f);

	for(i = 0; i < nr; i++)
	{
		fgets(nume_film, 100, f);
		nume_film[strlen(nume_film)-1] = '\0';

		fscanf(f, "%d", &x);
		fgets(ignore, 10, f);

		for(j = 0; j < x; j++)
		{
			fgets(nume_actor, 100, f);
			nume_actor[strlen(nume_actor)-1] = '\0';
			// Creez vectorul cu toti actorii
			if(duplicate(actori, nume_actor, nr_actori) < 0)
			{
				strcpy(actori[nr_actori++], nume_actor);
				strcpy(g->filme[nr_actori-1], nume_film);
			}
		}
	}

	if(c == '2')
	{
		char *ActorX = (char*)malloc(sizeof(char) * 100);
		char *ActorY = (char*)malloc(sizeof(char) * 100);

		fgets(ActorX, 100, f);
		ActorX[strlen(ActorX)-1] = '\0';
		fgets(ActorY, 100, f);
		ActorY[strlen(ActorY)-1] = '\0';

		g->ActorX = duplicate(actori, ActorX, nr_actori);
		g->ActorY = duplicate(actori, ActorY, nr_actori);

		free(ActorX);
		free(ActorY);
	}

	fclose(f);

	g->nr_actori = nr_actori;
	g->nr_filme = nr;
	g->adl = (TNode**)malloc(sizeof(TNode*) * nr_actori);
	for(i = 0; i < nr_actori; i++)
	{
		g->adl[i] = NULL;
		strcpy(g->actori[i], actori[i]);
	}

	f = fopen(fin, "r");

	fscanf(f, "%d", &nr);
	fgets(ignore, 10, f);

	for(i = 0; i < nr; i++)
	{
		fgets(nume_film, 100, f);
		nume_film[strlen(nume_film)-1] = '\0';

		fscanf(f, "%d", &x);
		fgets(ignore, 10, f);

		for(j = 0; j < x; j++)
		{
			fgets(nume_actor, 100, f);
			nume_actor[strlen(nume_actor)-1] = '\0';
			strcpy(sol[j], nume_actor); // Contruiesc actorii unui film
		}

		for(j = 0; j < x; j++)
		{
			for(k = j + 1; k < x; k++)
			{
				// Adaug legaturile dintre actori
				add_edge(g, sol[j], sol[k], actori, nume_film);
			}
		}
	}

	fclose(f);
	free(nume_film);
	free(nume_actor);
	free(ignore);
	for(i = 0; i < 500; i++)
	{
		free(sol[i]);
	}
	for(i = 0; i < 5000; i++)
	{
		free(actori[i]);
	}
	free(sol);
	free(actori);
	return g;
}

void dfs(TGraphL *g, int s, int *visit, int *x, char ***sol, char *actor)
{
	visit[s] = 1;
	(*x)++;
	strcpy((*sol)[(*x)-1], actor);
	for(TNode *q = g->adl[s]; q != NULL; q = q->next)
	{
		if(visit[q->v] == 0)
		{
			dfs(g, q->v, visit, x, sol, q->actor);
		}
	}
}

int compare(const void *a, const void *b)
{
	const char *pa = *(const char**)a;
	const char *pb = *(const char**)b;

	return strcmp(pa, pb);
}

void c1(TGraphL *g, FILE *fout)
{
	int *visit = malloc(sizeof(int) * g->nr_actori), i, nr = 0, x, max = -1, j;
	char **sol = (char**)malloc(sizeof(char*) * g->nr_actori);
	char **sol2 = (char**)malloc(sizeof(char*) * g->nr_actori);
	for(i = 0; i < g->nr_actori; i++)
	{
		sol[i] = (char*)malloc(sizeof(char) * 100);
		sol2[i] = (char*)malloc(sizeof(char) * 100);
	}

	for(i = 0; i < g->nr_actori; i++)
	{
		visit[i] = 0;
	}

	for(i = 0; i < g->nr_actori; i++)
	{
		if(visit[i] == 0)
		{
			nr++;
			x = 0;
			dfs(g, i, visit, &x, &sol, g->actori[i]);
			if(x > max)
			{
				max = x;	
				for(j = 0; j < max; j++)
				{
					strcpy(sol2[j], sol[j]);
				}

				qsort(sol2, max, sizeof(char*), compare);
			}
		}
	}
	
	fprintf(fout, "%d\n", max);
	for(i = 0; i < max; i++)
	{
		fprintf(fout, "%s\n", sol2[i]);
	}

	for(i = 0; i < g->nr_actori; i++)
	{
		free(sol[i]);
		free(sol2[i]);
	}
	free(sol);
	free(sol2);
}

int dublura_film(char **filme, char *film, int x)
{
	int i;

	for(i = 0; i < x; i++)
	{
		if(strcmp(filme[i], film) == 0) return 1;
	}

	return 0;
}

void Path(int *dad, int Y, char **filme, int *x, TGraphL *g)
{
	if(dad[Y] == -1) return;

	if(!dublura_film(filme, g->filme[dad[Y]], *x))
	{
		(*x)++;
		strcpy(filme[(*x)-1], g->filme[Y]);
	}

	Path(dad, dad[Y], filme, x, g);
}

void c2(TGraphL *g, FILE *fout)
{
	int *drum = (int*)malloc(sizeof(int) * g->nr_actori);
	int *dad = (int*)malloc(sizeof(int) * g->nr_actori);
	int i, min, grad = 0;
	MinHeap *h = newQueue(g->nr_actori);
	for(i = 0; i < g->nr_actori; i++)
	{
		drum[i] = 20000000;
		dad[i] = -1;
		h->elem[i] = newNode(i, drum[i]);
		h->pos[i] = i;
	}

	drum[g->ActorX] = 0;
	MinHeapNode* node = newNode(g->ActorX, drum[g->ActorX]);
	h->elem[0] = node;
	h->size++;

	while(!isEmpty(h))
	{
		min = removeMin(h)->v;
		for(TNode *p = g->adl[min]; p != NULL; p = p->next)
		{
			if(drum[min] + 1 < drum[p->v])
			{
				drum[p->v] = drum[min] + 1;
				dad[p->v] = min;
				MinHeapNode* node = newNode(p->v, drum[p->v]);
				h->elem[h->size] = node;
				h->size++;
				SiftUp(h, p->v, drum[p->v]);
			}
		}
	}

	if(drum[g->ActorY] == 20000000) fprintf(fout, "-1\n");
	else
	{
		char **filme = (char**)malloc(sizeof(char*) * g->nr_filme);
		for(i = 0; i < g->nr_filme; i++)
		{
			filme[i] = (char*)malloc(sizeof(char) * 100);
		}

		Path(dad, g->ActorY, filme, &grad, g);

		fprintf(fout, "%d\n", grad);
	}

	free(drum);
	free(dad);
}

void dfsB(TGraphL *g, int s, int *timp, int *idx, int *low, int *pi, int *nr, Punti **punti)
{
	idx[s] = *timp;
	low[s] = *timp;
	(*timp)++;

	for(TNode *p = g->adl[s]; p != NULL; p = p->next)
	{
		if(p->v != pi[s])
		{
			if(idx[p->v] == -1)
			{
				pi[p->v] = s;
				dfsB(g, p->v, timp, idx, low, pi, nr, punti);
				if(low[s] > low[p->v]) low[s] = low[p->v];
				if(low[p->v] > idx[s])
				{
					//Este punte
					(*nr)++;
					Punti *new = (Punti*)malloc(sizeof(Punti));
					new->Actor1 = (char*)malloc(sizeof(char) * 100);
					new->Actor2 = (char*)malloc(sizeof(char) * 100);
					if(strcmp(g->actori[s], p->actor) < 0)
					{
						strcpy(new->Actor1, g->actori[s]);
						strcpy(new->Actor2, p->actor);
					}
					else
					{
						strcpy(new->Actor1, p->actor);
						strcpy(new->Actor2, g->actori[s]);
					}
					new->next = NULL;

					if(*nr == 1)
					{
						(*punti) = new;
					}
					else
					{
						Punti *q = *punti, *prev = NULL;

						while(q != NULL && ((strcmp(q->Actor1, new->Actor1) < 0) || ((strcmp(q->Actor1, new->Actor1) == 0) && (strcmp(q->Actor2, new->Actor2) < 0))))
						{
							prev = q;
							q = q->next;
						}

						if(q)
						{
							if(prev == NULL)
							{
								new->next = *punti;
								*punti = new;
							}
							else
							{
								new->next = q;
								prev->next = new;
							}
						}
						else
						{
							prev->next = new;
						}
					}
				}
			}
			else
			{
				if(low[s] > low[p->v]) low[s] = low[p->v];
			}
		}
	}
}

void c3(TGraphL *g, FILE *fout)
{
	int timp = 0, i, nr = 0;
	int *idx = (int*)malloc(sizeof(int) * g->nr_actori);
	int *low = (int*)malloc(sizeof(int) * g->nr_actori);
	int *pi = (int*)malloc(sizeof(int) * g->nr_actori);
	Punti *punti = (Punti*)malloc(sizeof(Punti));

	for(i = 0; i < g->nr_actori; i++)
	{
		idx[i] = -1;
		low[i] = 20000000;
		pi[i] = -1;
	}

	for(i = 0; i < g->nr_actori; i++)
	{
		if(idx[i] == -1)
		{
			dfsB(g, i, &timp, idx, low, pi, &nr, &punti);
		}
	}

	fprintf(fout, "%d\n", nr);
	if(nr != 0)
	{
		Punti *q = punti;

		while(q)
		{
			fprintf(fout, "%s %s\n", q->Actor1, q->Actor2);
			q = q->next;
		}
	}
}

int main(int argc, char *argv[])
{
	FILE *fout = fopen(argv[3], "w");
	TGraphL *g;

	g = creare_graf(argv[2], argv[1][2]);

	switch (argv[1][2])
	{
		case '1':
			c1(g, fout);
			break;

		case '2':
			c2(g, fout);
			break;

		case '3':
			c3(g, fout);
			break;

		default:
			printf("Cerinta gresita");
			break;
	}

	fclose(fout);
	return 0;
}