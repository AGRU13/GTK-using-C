#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<gtk/gtk.h>
#include<unistd.h>

typedef struct list{
	char iname[128];
	struct list *next;
	struct list *prev;
}DCList;

DCList *current;
int count;
void Insert(char *fname){
	DCList *newNode=malloc(sizeof(DCList));
	if(!newNode){
		printf("\nMemory error!!");
		return;
	}
	strcpy(newNode->iname,fname);
	count++;
	newNode->next=newNode;
	newNode->prev=newNode;
	if(current==NULL){
		current=newNode;
	}else{
		newNode->next=current;
		newNode->prev=current->prev;
		current->prev->next=newNode;
		current->prev=newNode;
		current=newNode;
	}
}

void Delete(){
	DCList *temp=current;
	if(current==NULL){
		printf("\nEmpty List!!");
		return;
	}else if(current->next==current){
		current=NULL;
	}else{
		current->next->prev=current->prev;
		current->prev->next=current->next;
		current=current->next;	
	}
	count--;
	free(temp);
}

void Display(){
	if(count==0){
		printf("\nEmpty List!!");
		return;
	}
	printf("\nNumber of images: %d\n--\n",count);
	DCList *temp=current;
	do{
		printf("%s\n",temp->iname);
		temp=temp->next;
	}while(temp!=current);
}

static void insert_dialog(GtkWidget *widget,gpointer data){
	system("clear");
	GtkWidget *dialog,*table,*name,*lbn;
	gint result;
	dialog=gtk_dialog_new_with_buttons("Insert an image",NULL,GTK_DIALOG_MODAL,GTK_STOCK_OK,GTK_RESPONSE_OK,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(dialog),GTK_RESPONSE_OK);
	lbn=gtk_label_new("Enter the name of the image: ");
	name=gtk_entry_new();
	table=gtk_table_new(1,1,FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table),lbn,0,1,0,1);
	gtk_table_attach_defaults(GTK_TABLE(table),name,1,2,0,1);
	gtk_table_set_row_spacings(GTK_TABLE(table),5);
	gtk_table_set_col_spacings(GTK_TABLE(table),5);
	gtk_container_set_border_width(GTK_CONTAINER(table),5);
	gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox),table);
	gtk_widget_show_all(dialog);
	result=gtk_dialog_run(GTK_DIALOG(dialog));
	if(result==GTK_RESPONSE_OK){
		if(access(gtk_entry_get_text(GTK_ENTRY(name)),F_OK)==-1)
			printf("\nFile does not exist!!");
		else{
			Insert(gtk_entry_get_text(GTK_ENTRY(name)));
			gtk_image_set_from_file(GTK_IMAGE(data),current->iname);
		}
	}
	gtk_widget_destroy(dialog);
	Display();
}

static void delete_dialog(GtkWidget *widget,gpointer data){
	system("clear");
	Delete();
	if(current)
		gtk_image_set_from_file(GTK_IMAGE(data),current->iname);
	else
		gtk_image_set_from_file(GTK_IMAGE(data),NULL);
	Display();
}

static void next_dialog(GtkWidget *widget,gpointer data){
	if(count==0) return;
	current=current->next;
	gtk_image_set_from_file(GTK_IMAGE(data),current->iname);
}

static void prev_dialog(GtkWidget *widget,gpointer data){
	if(count==0) return;
	current=current->prev;
	gtk_image_set_from_file(GTK_IMAGE(data),current->iname);
}

int main(int argc,char *argv[]){
	system("clear");
	current=NULL;
	count=0;
	GtkWidget *window,*table,*delete,*insert,*next,*prev,*image;
	gtk_init(&argc,&argv);
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"PHOTO VIEWER");
	gtk_container_set_border_width(GTK_CONTAINER(window),10);
	g_signal_connect(window,"delete-event",G_CALLBACK(gtk_main_quit),NULL);	
	table=gtk_table_new(3,2,FALSE);
	image=gtk_image_new_from_file(NULL);
	delete=gtk_button_new_with_mnemonic("_Delete current image");
	g_signal_connect(delete,"clicked",G_CALLBACK(delete_dialog),image);
	insert=gtk_button_new_with_mnemonic("_Insert an image");
	next=gtk_button_new_with_mnemonic("_Next _Image");
	prev=gtk_button_new_with_mnemonic("_Previous _Image");
	g_signal_connect(next,"clicked",G_CALLBACK(next_dialog),image);
	g_signal_connect(prev,"clicked",G_CALLBACK(prev_dialog),image);
	g_signal_connect(insert,"clicked",G_CALLBACK(insert_dialog),image);
	gtk_button_set_relief(GTK_BUTTON(insert),GTK_RELIEF_NORMAL);
	gtk_button_set_relief(GTK_BUTTON(delete),GTK_RELIEF_NORMAL);
	gtk_button_set_relief(GTK_BUTTON(next),GTK_RELIEF_NORMAL);
	gtk_button_set_relief(GTK_BUTTON(prev),GTK_RELIEF_NORMAL);
	gtk_table_attach(GTK_TABLE(table),insert,0,1,0,1,GTK_EXPAND,GTK_EXPAND,0,0);
	gtk_table_attach(GTK_TABLE(table),delete,1,2,0,1,GTK_EXPAND,GTK_EXPAND,0,0);
	gtk_table_attach(GTK_TABLE(table),prev,0,1,1,2,GTK_EXPAND,GTK_EXPAND,0,0);
	gtk_table_attach(GTK_TABLE(table),next,1,2,1,2,GTK_EXPAND,GTK_EXPAND,0,0);
	gtk_table_attach(GTK_TABLE(table),image,0,2,2,3,GTK_EXPAND,GTK_EXPAND,0,0);
	gtk_table_set_row_spacings(GTK_TABLE(table),5);
	gtk_table_set_col_spacings(GTK_TABLE(table),5);
	gtk_container_add(GTK_CONTAINER(window),table);
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}
	
