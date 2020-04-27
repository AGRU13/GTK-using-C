#include<stdio.h>
#include<stdlib.h>
#include<gtk/gtk.h>
#include<string.h>
#include<unistd.h>

typedef struct list{
	char iname[128];
	struct list *next;
}CList;

CList *head=NULL,*slide;
int count=0;
void Insert(int position,char *fname){
	position-='0';
	CList *current=head,*newNode=malloc(sizeof(CList));
	int k=1;
	if(!newNode){
		printf("Memory error!!");
		return;
	}
	strcpy(newNode->iname,fname);
	count++;
	newNode->next=newNode;
	if(head==NULL){
		head=newNode;
	}else if(position==1){
		while(current->next!=head)
			current=current->next;
		newNode->next=head;
		current->next=newNode;
		head=newNode;
	}else{
		while(k<position-1&&current->next!=head){
			k++;
			current=current->next;
		}
		if(k!=position-1)
			printf("Position does not exist,Inserting in the end!!");
		newNode->next=current->next;
		current->next=newNode;
	}
}

void Delete(int position){
	position-='0';
	CList *temp,*current;
	int k=1;
	if(head==NULL){
		printf("\nEmpty List!!");
		return;
	}
	current=temp=head;
	if(position==1){
		while(current->next!=head)
			current=current->next;
		current->next=(head)->next;
		head=head->next;
		free(temp);
		count--;
	}else if(position>count){
		printf("\nPosition does not exist!!");
		return;
	}else{
		while(k<position-1&&current->next!=head){
			k++;
			current=current->next;
		}
		if(k!=position-1)
			printf("\nPosition does not exist!!");
		else{
			temp=current->next;
			current->next=temp->next;
			free(temp);
			count--;
		}
	}
}

void Display(){
	if(count==0){
		printf("\nEmpty List!!");
		return;
	}else{
		printf("\nNumber of images: %d\n--\n",count);
		CList *temp=head;
		do{
			printf(" %s\n",temp->iname);
			temp=temp->next;
		}while(temp!=head);
	}
}

static gboolean show(gpointer data){	
	if(slide->next==head){
		gtk_image_set_from_file(GTK_IMAGE(data),NULL);
	       	return FALSE;
	}
	else{
		slide=slide->next;
		gtk_image_set_from_file(GTK_IMAGE(data),slide->iname);
		return TRUE;
	}
}

static void slideshow(GtkWidget *widget,gpointer data){
	slide=head;
	if(slide==NULL) return;
	gtk_image_set_from_file(GTK_IMAGE(data),slide->iname);
	gint temp=g_timeout_add(1000,show,data);
	return;
}

static void insert_dialog(GtkWidget *widget,gpointer data){
	system("clear");
	GtkWidget *dialog,*table,*name,*pos,*lbn,*lbp;
	gint result;
	dialog=gtk_dialog_new_with_buttons("Insert an image",NULL,GTK_DIALOG_MODAL,GTK_STOCK_OK,GTK_RESPONSE_OK,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(dialog),GTK_RESPONSE_OK);
	lbn=gtk_label_new("Enter the name of the image: ");
	lbp=gtk_label_new("Enter the position of the image: ");
	name=gtk_entry_new();
	pos=gtk_entry_new();
	table=gtk_table_new(2,2,FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table),lbn,0,1,0,1);
	gtk_table_attach_defaults(GTK_TABLE(table),lbp,0,1,1,2);
	gtk_table_attach_defaults(GTK_TABLE(table),name,1,2,0,1);
	gtk_table_attach_defaults(GTK_TABLE(table),pos,1,2,1,2);
	gtk_table_set_row_spacings(GTK_TABLE(table),5);
	gtk_table_set_col_spacings(GTK_TABLE(table),5);
	gtk_container_set_border_width(GTK_CONTAINER(table),5);
	gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox),table);
	gtk_widget_show_all(dialog);
	result=gtk_dialog_run(GTK_DIALOG(dialog));
	if(result==GTK_RESPONSE_OK){
		char *p=gtk_entry_get_text(GTK_ENTRY(pos));
		if(access(gtk_entry_get_text(GTK_ENTRY(name)),F_OK)==-1)
			printf("\nFile does not exist!!");
		else
		Insert(*p,gtk_entry_get_text(GTK_ENTRY(name)));
	}
	gtk_widget_destroy(dialog);
	Display();
}

static void delete_dialog(GtkWidget *widget,gpointer data){
	system("clear");
	GtkWidget *dialog,*table,*pos,*lbp;
	gint result;
	dialog=gtk_dialog_new_with_buttons("Delete an image",NULL,GTK_DIALOG_MODAL,GTK_STOCK_OK,GTK_RESPONSE_OK,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(dialog),GTK_RESPONSE_OK);
	lbp=gtk_label_new("Enter the position of the image: ");
	pos=gtk_entry_new();
	table=gtk_table_new(1,1,FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table),lbp,0,1,0,1);
	gtk_table_attach_defaults(GTK_TABLE(table),pos,1,2,0,1);
	gtk_table_set_row_spacings(GTK_TABLE(table),5);
	gtk_table_set_col_spacings(GTK_TABLE(table),5);
	gtk_container_set_border_width(GTK_CONTAINER(table),5);
	gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox),table);
	gtk_widget_show_all(dialog);
	result=gtk_dialog_run(GTK_DIALOG(dialog));
	if(result==GTK_RESPONSE_OK){
		char *p=gtk_entry_get_text(GTK_ENTRY(pos));
		Delete(*p);
	}
	gtk_widget_destroy(dialog);
	Display();
}

int main(int argc, char *argv[]){
	system("clear");
	GtkWidget *window,*delete,*insert,*start,*table,*image;
	gtk_init(&argc,&argv);
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"SLIDESHOW");
	gtk_container_set_border_width(GTK_CONTAINER(window),10);
	g_signal_connect(window,"delete-event",G_CALLBACK(gtk_main_quit),NULL);	
	table=gtk_table_new(3,2,FALSE);
	delete=gtk_button_new_with_mnemonic("_Delete an image");
	g_signal_connect(delete,"clicked",G_CALLBACK(delete_dialog),NULL);
	insert=gtk_button_new_with_mnemonic("_Insert an image");
	g_signal_connect(insert,"clicked",G_CALLBACK(insert_dialog),NULL);
	start=gtk_button_new_with_mnemonic("_Start the slideshow");
	image=gtk_image_new_from_file(NULL);
	gtk_button_set_relief(GTK_BUTTON(insert),GTK_RELIEF_NORMAL);
	gtk_button_set_relief(GTK_BUTTON(start),GTK_RELIEF_NORMAL);
	g_signal_connect(start,"clicked",G_CALLBACK(slideshow),image);
	gtk_table_attach(GTK_TABLE(table),insert,0,1,0,1,GTK_EXPAND,GTK_EXPAND,0,0);
	gtk_table_attach(GTK_TABLE(table),delete,1,2,0,1,GTK_EXPAND,GTK_EXPAND,0,0);
	gtk_table_attach(GTK_TABLE(table),start,0,2,1,2,GTK_EXPAND,GTK_EXPAND,0,0);
	gtk_table_attach(GTK_TABLE(table),image,0,2,2,3,GTK_EXPAND,GTK_EXPAND,0,0);
	gtk_table_set_row_spacings(GTK_TABLE(table),5);
	gtk_table_set_col_spacings(GTK_TABLE(table),5);
	gtk_container_add(GTK_CONTAINER(window),table);
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}
