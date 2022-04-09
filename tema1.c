#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir
{
	char *name;						  // numele directorului
	struct Dir *parent;				  // pointer catre parintele directorului(null pentru radacina)
	struct File *head_children_files; // pointer catre primul element de tip File din interiorul directorului
	struct Dir *head_children_dirs;	  // pointer catre primul element de tip Dir din interiorul directorului
	struct Dir *next;
} Dir; // structura de tip director

typedef struct File
{
	char *name;			// numele fisierului
	struct Dir *parent; // pointer catre directorul pe
	struct File *next;	// pointer catre urmatorul element din lista de fisiere
} File;					// structura de tip fisier

void touch(Dir *parent, char *name)
{
	//daca pun pe prima pozitie
	if (parent->head_children_files == NULL)
	{
		File *file;
		file = (File *)malloc(sizeof(File));
		file->name = (char*)malloc(1000*sizeof(char));
		strcpy(file->name, name);
		file->next = NULL;
		file->parent = parent;
		parent->head_children_files = file;
	}
	else
	{
		File *file;
		file = (File *)malloc(sizeof(File));
		file->name = (char*)malloc(1000*sizeof(char));
		strcpy(file->name, name);
		file->parent = parent;
		File *current = parent->head_children_files;
		//caut ultimul loc unde sa pun fisierul
		while (current->next != NULL)
		{
			//am grija sa nu existe deja
			if (strcmp(current->name, name) == 0)
			{
				printf("%s\n", "File already exists");
				free(file->name);
				free(file);
				return;
			}
			current = current->next;
		}
		//daca next e null, atunci sunt chiar pe ultimul nod
		if (strcmp(current->name, name) == 0)
		{
			printf("%s\n", "File already exists");
			free(file->name);
			free(file);
			return;
		}
		current->next = file;
		file->next = NULL;
		return;
	}
}

void mkdir(Dir *parent, char *name)
{
	
	if (parent->head_children_dirs == NULL)
	{
		Dir *new;
		new = (Dir *)malloc(sizeof(Dir));
		new->head_children_dirs = NULL;
		new->head_children_files = NULL;
		new->name = (char*)malloc(1000*sizeof(char));
		strcpy(new->name, name);
		new->next = NULL;
		new->parent = parent;
		parent->head_children_dirs = new;
	}
	else
	{
		Dir *new;
		new = (Dir *)malloc(sizeof(Dir));
		new->head_children_dirs = NULL;
		new->head_children_files = NULL;
		new->name = (char*)malloc(1000*sizeof(char));
		strcpy(new->name, name);
		new->parent = parent;
		Dir *current = parent->head_children_dirs;
		while (current->next != NULL)
		{
			if (strcmp(current->name, name) == 0)
			{
				printf("%s\n", "Directory already exists");
				free(new->name);
				free(new);
				return;
			}
			current = current->next;
		}
		
		if (strcmp(current->name, name) == 0)
		{
			printf("%s\n", "Directory already exists");
			free(new->name);
			free(new);
			return;
		}
		current->next = new;
		new->next = NULL;
		return;
	}
}

void ls(Dir *parent)
{
	Dir *current = parent->head_children_dirs;
	if (current != NULL)
	{
		while (current != NULL)
		{
			printf("%s\n", current->name);
			current = current->next;
		}
	}

	Dir *current2 = parent;
	if (current2->head_children_files != NULL)
	{
		File *thisFile = current2->head_children_files;
		while (thisFile != NULL)
		{
			printf("%s\n", thisFile->name);
			thisFile = thisFile->next;
		}
	}
}

void cd(Dir **target, char *name)
{
	if (strcmp(name, "..") == 0)
	{
		Dir *current = (*target);
		if (strcmp(current->name, "home") == 0)
			return;
		else
		{
			(*target) = current->parent;
			return;
		}
	}
	else
	{
		Dir *current = (*target)->head_children_dirs;
		if(current==NULL){
			printf("No directories found!\n");
			return;
		}
		if (strcmp(current->name, name) == 0)
		{
			(*target) = current;
			return;
		}
		while (current != NULL && strcmp(current->name, name) != 0)
		{
			current = current->next;
			if (current == NULL)
			{
				printf("No directories found!\n");
				return;
			}
			if (strcmp(current->name, name) == 0)
			{
				(*target) = current;
				return;
			}
		}
	}
}


void rm(Dir *parent, char *name)
{
	
	File *current = parent->head_children_files;
	if (current != NULL)
	{
		if (strcmp(current->name, name) == 0)
		{
			if (current->next != NULL)
				parent->head_children_files = parent->head_children_files->next;
			else
				parent->head_children_files = NULL;
			free(current->name);
			free(current);
			return;
		}
		while (current!=NULL && current->next != NULL && strcmp(current->next->name, name) != 0)
			current = current->next;
		if (current!=NULL && current->next != NULL)
		{
			File *toBeDeleted = current->next;
			current->next = current->next->next;
			free(toBeDeleted->name);
			free(toBeDeleted);
			return;
		}
		else
			printf("Could not find the file\n");
	}
	else
		printf("Could not find the file\n");
}

//am schimbat in this pentru ca ma incurc in parent
//eu am gandit codul astfel incat sa stearga chiar directorul precizat, punand cursorul fix pe el
void rmdir(Dir *this, char *name)
{
	Dir *firstNEXT;
	//Dir *delete;
	if (this == NULL)
	{
		printf("%s\n", "Could not find the dir");
		return;
	}
	//apelez recursiv pana ajung la directorul meu
	if (strcmp(this->name, name) != 0)
		rmdir(this->next, name);
	else
	{
		Dir *delete;
		//daca e chiar primul director din head child dirs fac chestii
		if (this->parent!=NULL && strcmp(this->parent->head_children_dirs->name, this->name) == 0)
		{
			if (this->next != NULL)
				this->parent->head_children_dirs = this->next;
			else
				this->parent->head_children_dirs = NULL;
			delete=this;
		}
		//daca nu, il caut 
		if(this->parent!=NULL){
		Dir *current = this->parent->head_children_dirs;
		while (current !=NULL && current->next != NULL && strcmp(current->next->name, name) != 0)
			current = current->next;
		if (current!=NULL && current->next != NULL)
		{
			Dir *toBeDeleted = current->next;
			current->next = toBeDeleted->next;
			delete=toBeDeleted;
		}
		}
	    //salvez in deleted adresa lui ca sa o sterg, desi am modificat in lista de directoare
		//vad daca am fisiere si le sterg
		if (this->head_children_files != NULL)
		{
			//File *nodulCurent = this->head_children_files;
			File *nextNode;
			File *first1=this->head_children_files;

			while (first1 != NULL)
			{
				nextNode = first1->next;
				free(first1->name);
				free(first1);
				first1 = nextNode;
			}
		}
		this->head_children_files=NULL;
		//vad daca mai am copii in directorul asta pe care vreau sa il sterg
		Dir *first = this->head_children_dirs;
		while (first != NULL )
		{
			//merg si pe next-ii din copii lui
			firstNEXT=first->next;
			rmdir(first, first->name);
			first=firstNEXT;
		}
		//sterg adresa
		
			free(delete->name);
			free(delete);
		
	}
}

char *pwd(Dir *target)
{
	if (target == NULL)
		return NULL;
	else
	{
		pwd(target->parent);
		printf("/%s", target->name);
	}
	//afisez '\n' in main
}


void tree(Dir *target, int level)
{
	if (target == NULL)
		return;
	if (target != NULL)
	{
		for (int i = 1; i < level; i++)
			printf("    ");
		//nu vreau level 0 pentru ca nu vreau sa afisez si home-ul 
		if(level>0)
		    printf("%s\n", target->name);
		tree(target->head_children_dirs, ++level);
		if (target->head_children_dirs != NULL)
		{
			Dir *curr = target->head_children_dirs->next;
			while (curr != NULL)
			{
				for (int i = 1; i < level; i++)
					printf("    ");
				printf("%s\n", curr->name);
				curr = curr->next;
			}
		}

		if (target->head_children_files != NULL)
		{
			File *curr = target->head_children_files;
			while (curr != NULL)
			{
				for (int i = 1; i < level; i++)
					printf("    ");
				printf("%s\n", curr->name);
				curr = curr->next;
			}
		}
	}

	return;
}

void mv(Dir* parent, char* oldname, char* newname){
	Dir *curr=parent->head_children_dirs;
	Dir *curr2=parent->head_children_dirs;
	File *aux = parent->head_children_files;
	File *aux2 = parent->head_children_files;
	int ok=0;
	while(curr2!=NULL){	
		if(strcmp(curr2->name, newname)==0){
			printf("File/Director already exists\n");
			return;
		}
		curr2=curr2->next;
	}
	while(aux2!=NULL){
		
		if(strcmp(aux2->name, newname)==0){
			printf("File/Director already exists\n");
			return;
		}
		aux2=aux2->next;
	}
	while(curr!=NULL){
		if(strcmp(curr->name, oldname)==0)
			break;
		curr=curr->next;
	}
	
	if(curr==NULL)
		ok++;
	if(ok==0){
		if(strcmp(curr->name, parent->head_children_dirs->name)==0 && parent->head_children_dirs->next==NULL){
			strcpy(curr->name,newname);
			return;
		}
		//Dir *new=(Dir*)(malloc(sizeof(Dir)));
		/*new->head_children_dirs=curr->head_children_dirs;
		new->head_children_files=curr->head_children_files;
		new->next=NULL;
		new->parent=curr->parent;
		new->name =(char*)malloc(1000*sizeof(char));
		strcpy(new->name, newname);
		rmdir(curr, curr->name);
		Dir *moved=parent->head_children_dirs;
		while(moved->next!=NULL){
			moved=moved->next;
		}
		moved->next=new;
		return; */
		Dir * moved, *k, *copy;
		moved = parent -> head_children_dirs;
		if(parent -> head_children_dirs && strcmp(parent->head_children_dirs->name, oldname)==0){
			copy = parent->head_children_dirs;
			parent ->head_children_dirs = copy -> next;
			while ( moved && moved -> next) {
				moved = moved -> next;
			}
			moved -> next = copy;
			copy -> next = NULL;
			strcpy(copy->name, newname);
			return;
		}
		else {
		while ( moved && moved -> next && strcmp(moved -> next -> name, oldname) != 0) {
			moved = moved -> next;
		}
		if (moved ) {
		k = moved ->next;
		 moved -> next = k -> next;
		k -> next = NULL;
		}
		moved = parent->head_children_dirs;
		while ( moved && moved -> next) {
			moved = moved -> next;
		}
		if (moved) {
		moved -> next = k;
		}
		strcpy ( k -> name, newname);
	 }
	 return;
	}
	while(aux!=NULL){
		if(strcmp(aux->name, oldname)==0)
			break;
		aux=aux->next;
	}
	
	if(aux==NULL)
		ok++;
	if(ok==1){
		if(aux && strcmp(aux->name, parent->head_children_files->name)==0 && parent->head_children_files->next==NULL){
			strcpy(aux->name,newname);
			return;
		}
		File *new=(File*)malloc(sizeof(File));
		new->name =(char*)malloc(1000*sizeof(char));
		strcpy(new->name, newname);
		new->next=NULL;
		new->parent=parent;
		if ( aux )
		rm(aux->parent, aux->name);
		File *moved=parent->head_children_files;
		if(moved!=NULL) {
		while(moved->next=NULL){
			moved=moved->next;
		}
		moved->next=new;
		}
		return;
	}
	if(ok!=0){
		printf("File/Director not found\n");
	}

}
void stop(Dir *target)
{
	//aplic rmdir pe toate directoarele din home, in ordine
	//si streg si fisierele tot de mana
	while(target->head_children_dirs!=NULL)
		rmdir(target->head_children_dirs,target->head_children_dirs->name);

	File *currFile=target->head_children_files;
	while(target->head_children_files!=NULL)
	{
		currFile=target->head_children_files;
		target->head_children_files=target->head_children_files->next;
		free(currFile->name);
		free(currFile);
	}
    free(target->name);
	free(target);
}

int main()
{
	char buffer[MAX_INPUT_LINE_SIZE];
	Dir *home;
	int ok = 0;
	home = (Dir *)malloc(sizeof(Dir));
	home->next = NULL;
	home->name = (char*)malloc(100*sizeof(char));;
	strcpy(home->name, "home");
	home->head_children_dirs = NULL;
	home->head_children_files = NULL;
	home->parent = NULL;
	Dir *cursor = home;
	
	char *linie =(char*)malloc(101 * sizeof(char));
	char *cuv= (char*)malloc(101 * sizeof(char));
	
	while (fgets(buffer, MAX_INPUT_LINE_SIZE, stdin) != NULL && ok == 0)
	{
		
		buffer[strlen(buffer) - 1] = '\0';
		strcpy(linie, buffer);
		char *pch= strtok(linie, " ");
		if (pch == NULL)
			strcpy(cuv, buffer);
		else
			strcpy(cuv, pch);
		int alfa=100;
		if (strcmp(cuv, "touch") == 0)
			alfa = 0;
		if (strcmp(cuv, "mkdir") == 0)
			alfa = 1;
		if (strcmp(cuv, "ls") == 0)
			alfa = 2;
		if (strcmp(cuv, "rm") == 0)
			alfa = 3;
		if (strcmp(cuv, "rmdir") == 0)
			alfa = 4;
		if (strcmp(cuv, "cd") == 0)
			alfa = 5;
		if (strcmp(cuv, "tree") == 0)
			alfa = 6;
		if (strcmp(cuv, "pwd") == 0)
			alfa = 7;
		if (strcmp(cuv, "stop") == 0)
			alfa = 8;
		if (strcmp(cuv, "mv") == 0)
			alfa = 9;
		switch (alfa)
		{
		case 0:
			pch = strtok(NULL, " ");
			touch(cursor, pch);
			break;
		case 1:
			pch = strtok(NULL, " ");
			mkdir(cursor, pch);
			break;
		case 2:
			ls(cursor);
			break;
		case 3:
			pch = strtok(NULL, " ");
			rm(cursor, pch);
			break;
		case 4:
			pch = strtok(NULL, " ");
			rmdir(cursor->head_children_dirs, pch);
			break;
		case 5:
			pch = strtok(NULL, " ");
			cd(&cursor, pch);
			break;
		case 6:
			tree(cursor, 0);
			break;
		case 7:
			pwd(cursor);
			printf("\n");
			break;
		case 8:
			stop(home);
			free(linie);
	        free(cuv);
	        return 0;
			break;
		case 9:
			pch = strtok(NULL, " ");
			char *aux = strtok(NULL, " ");
			mv(cursor, pch, aux);
			break;
		case 100: break;
		
		}
	}
}
