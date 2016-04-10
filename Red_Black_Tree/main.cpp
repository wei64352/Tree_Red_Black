#include"stdio.h"
#include"stdlib.h"
#include"string.h"

enum RedBlack{
	red, black
};
typedef struct TreeNode{
	int value;
	bool color;
	TreeNode* r_chlid;
	TreeNode* l_chlid;
}tree_node;
typedef struct NodePoint{
	tree_node* point;
}n_ptr;

int Load_file(char* file_path, int* data_buf);
tree_node* Build_tree(int* data_buf, int data_size, tree_node* root);
tree_node* Create_node(int value);
bool Search_node(n_ptr* n_node, n_ptr* node, n_ptr* p_node, n_ptr* gp_node, n_ptr* an_node, n_ptr* rtp);
int Insert_node(tree_node* n_node, tree_node* node);
void Change_color(tree_node* node);
void Check_continue_red(n_ptr* node, n_ptr* p_node, n_ptr* gp_node, n_ptr* an_node, n_ptr* rtp);
void Node_rotation(int type, n_ptr* node, n_ptr* p_node, n_ptr* gp_node, n_ptr* an_node, n_ptr* rtp);

bool Binary_search(tree_node* node, int value, n_ptr* ptr);

void Print_all_tree_value(tree_node* root);
void DF_search(tree_node* node, FILE* fout);

int main(int argc, char* argv[]){
	//char file_path[500];
	//printf("Enter file path: ");
	//scanf("%500s", file_path);
	int data_buf[2000];
	int size = Load_file("data.txt", data_buf);
	//int size = Load_file(file_path, data_buf);
	if (size > 0){
		tree_node* root = NULL;
		root = Build_tree(data_buf, size, root);
		Print_all_tree_value(root);
	}
	system("pause");
	return 0;
}
int Load_file(char* file_path, int* data_buf){
	FILE* fin = fopen(file_path, "r");
	if (NULL != fin){
		char data[10000];
		char* cut = " ";
		char* output;
		int index = 0;
		fread(data, 1, sizeof(data), fin);
		output = strtok(data, cut);
		while (output){
			data_buf[index] = atoi(output);
			output = strtok(NULL, cut);
			index++;
		}
		fclose(fin);
		printf("Open file successful...\n");
		return index;
	}else{
		printf("File Error: \"%s\" isn't exist...\n");
		return false;
	}
}
tree_node* Build_tree(int* data_buf, int size, tree_node* root){
	bool bol;
	int re_data[sizeof(data_buf - 1)], index = 0;
	root = Create_node(data_buf[0]);
	n_ptr* rt_ptr = (n_ptr*)malloc(sizeof(n_ptr*));
	n_ptr* ptr = (n_ptr*)malloc(sizeof(n_ptr*));
	rt_ptr->point = root;
	for (int i = 1; i < size; i++){
		tree_node * n_node = Create_node(data_buf[i]);
		ptr->point = n_node;
		bol = Search_node(ptr, rt_ptr, NULL, NULL, NULL, rt_ptr);
		if (bol == false){
			re_data[index] = n_node->value;
			index++;
		}
	}
	root = rt_ptr->point;
	if (index != 0){
		printf(" data: %d\n\n", re_data[0]);
		for (int i = 1; i < index; i++){
			printf(", %d", re_data[i]);
		}
	}else{
		printf("No repeated...\n\n");
	}
	free(rt_ptr);
	free(ptr);
	return root;
}
tree_node* Create_node(int value){
	tree_node* n_node = (tree_node*)malloc(sizeof(tree_node*));
	n_node->value = value;
	n_node->color = red;
	n_node->l_chlid = NULL;
	n_node->r_chlid = NULL;
	return n_node;
}
bool Search_node(n_ptr* n_node, n_ptr* node, n_ptr* p_node, n_ptr* gp_node, n_ptr* an_node, n_ptr* rtp){
	bool node_suc;
	if (gp_node != NULL){
		Change_color(p_node->point);
		Check_continue_red(node, p_node, gp_node, an_node, rtp);
		int type = Insert_node(n_node->point, node->point);
		n_ptr* ptr = (n_ptr*)malloc(sizeof(n_ptr*));
		switch (type)
		{
		case 0:
			ptr->point = n_node->point;
			Check_continue_red(ptr, node, p_node, gp_node, rtp);
			node_suc = true;
			break;
		case 1:
			ptr->point = node->point->l_chlid;
			node_suc = Search_node(n_node, ptr, node, p_node, gp_node, rtp);
			Check_continue_red(node, p_node, gp_node, an_node, rtp);
			break;
		case 2:
			ptr->point = node->point->r_chlid;
			node_suc = Search_node(n_node, ptr, node, p_node, gp_node, rtp);
			Check_continue_red(node, p_node, gp_node, an_node, rtp);
			break;
		case 3:
			node_suc = false;
			break;
		default:
			break;
		}free(ptr);
	}
	else if (p_node != NULL){
		Change_color(p_node->point);
		p_node->point->color = black;
		int type = Insert_node(n_node->point, node->point);
		n_ptr* ptr = (n_ptr*)malloc(sizeof(n_ptr*));
		switch (type)
		{
		case 0:
			ptr->point = n_node->point;
			Check_continue_red(ptr, node, p_node, NULL, rtp);
			node_suc = true;
			break;
		case 1:
			ptr->point = node->point->l_chlid;
			node_suc = Search_node(n_node, ptr, node, p_node, NULL, rtp);
			break;
		case 2:
			ptr->point = node->point->r_chlid;
			node_suc = Search_node(n_node, ptr, node, p_node, NULL, rtp);
			break;
		case 3:
			node_suc = false;
			break;
		default:
			break;
		}free(ptr);
	}
	else{
		node->point->color = black;
		int type = Insert_node(n_node->point, node->point);
		n_ptr* ptr = (n_ptr*)malloc(sizeof(n_ptr*));
		switch (type)
		{
		case 0:
			node_suc = true;
			break;
		case 1:
			ptr->point = node->point->l_chlid;
			node_suc = Search_node(n_node, ptr, node, NULL, NULL, rtp);
			break;
		case 2:
			ptr->point = node->point->r_chlid;
			node_suc = Search_node(n_node, ptr, node, NULL, NULL, rtp);
			break;
		case 3:
			node_suc = false;
			break;
		default:
			break;
		}free(ptr);
	}return node_suc;
}
int Insert_node(tree_node* n_node, tree_node* node){
	if (n_node->value < node->value){
		if (node->l_chlid != NULL){ return 1; }
		else{ node->l_chlid = n_node; }
	}
	else if (n_node->value > node->value){
		if (node->r_chlid != NULL){ return 2; }
		else{ node->r_chlid = n_node; }
	}
	else{ return 3; }
	return 0;
}
void Change_color(tree_node* node){
	if (node->l_chlid != NULL && node->r_chlid != NULL){
		if (node->l_chlid->color == red && node->r_chlid->color == red){
			node->color = red;
			node->l_chlid->color = black;
			node->r_chlid->color = black;
		}
	}
}
void Check_continue_red(n_ptr* node, n_ptr* p_node, n_ptr* gp_node, n_ptr* an_node, n_ptr* rtp){
	if (node->point->color == red && p_node->point->color == red){
		if (gp_node->point->l_chlid == p_node->point && p_node->point->l_chlid == node->point){
			Node_rotation(0, node, p_node, gp_node, an_node, rtp);
		}else if (gp_node->point->l_chlid == p_node->point && p_node->point->r_chlid == node->point){
			Node_rotation(1, node, p_node, gp_node, an_node, rtp);
		}else if (gp_node->point->r_chlid == p_node->point && p_node->point->l_chlid == node->point){
			Node_rotation(2, node, p_node, gp_node, an_node, rtp);
		}else{
			Node_rotation(3, node, p_node, gp_node, an_node, rtp);
		}
	}
}
void Node_rotation(int type, n_ptr* node, n_ptr* p_node, n_ptr* gp_node, n_ptr* an_node, n_ptr* rtp){
	n_ptr* ptr = (n_ptr*)malloc(sizeof(n_ptr*));
	switch (type)
	{
	case 0:
		if (p_node->point->r_chlid != NULL){
			gp_node->point->l_chlid = p_node->point->r_chlid;
		}else{ gp_node->point->l_chlid = NULL; }
		p_node->point->r_chlid = gp_node->point;
		p_node->point->color = black;
		gp_node->point->color = red;
		break;
	case 1:
		if (node->point->l_chlid != NULL){
			p_node->point->r_chlid = node->point->l_chlid;
		}else{ p_node->point->r_chlid = NULL; }
		if (node->point->r_chlid != NULL){
			gp_node->point->l_chlid = node->point->r_chlid;
		}else{ gp_node->point->l_chlid = NULL; }
		node->point->l_chlid = p_node->point;
		node->point->r_chlid = gp_node->point;
		node->point->color = black;
		gp_node->point->color = red;
		ptr->point = node->point;
		node->point = p_node->point;
		p_node->point = ptr->point;
		break;
	case 2:
		if (node->point->l_chlid != NULL){
			gp_node->point->r_chlid = node->point->l_chlid;
		}else{ gp_node->point->r_chlid = NULL; }
		if (node->point->r_chlid != NULL){
			p_node->point->l_chlid = node->point->r_chlid;
		}else{ p_node->point->l_chlid = NULL; }
		node->point->l_chlid = gp_node->point;
		node->point->r_chlid = p_node->point;
		node->point->color = black;
		gp_node->point->color = red;
		ptr->point = node->point;
		node->point = p_node->point;
		p_node->point = ptr->point;
		break;
	case 3:
		if (p_node->point->l_chlid != NULL){
			gp_node->point->r_chlid = p_node->point->l_chlid;
		}else{ gp_node->point->r_chlid = NULL; }
		p_node->point->l_chlid = gp_node->point;
		p_node->point->color = black;
		gp_node->point->color = red;
		break;
	default:
		break;
	}free(ptr);
	if (an_node != NULL){
		if (an_node->point->l_chlid == gp_node->point){
			an_node->point->l_chlid = p_node->point;
		}
		else{ an_node->point->r_chlid = p_node->point; }
		gp_node->point = an_node->point;
		an_node = NULL;
	}
	else{
		rtp->point = p_node->point;
		gp_node = NULL;
	}
}

bool Binary_search(tree_node* node, int value, n_ptr* ptr){
	if (value > node->value){
		if (node->r_chlid != NULL){
			Binary_search(node->r_chlid, value, ptr);
		}else{ return false; }
	}
	else if(value < node->value){
		if (node->l_chlid != NULL){
			Binary_search(node->l_chlid, value, ptr);
		}else{ return false; }
	}
	else{
		ptr->point = node;
		return true;
	}
}

void Print_all_tree_value(tree_node* root){
	FILE* fout = fopen("Red-Black tree.txt", "w");
	DF_search(root, fout);
	fclose(fout);
}
void DF_search(tree_node* node, FILE* fout){
	if (node->l_chlid != NULL){
		DF_search(node->l_chlid, fout);
		printf("%d, %d\n", node->value, node->color);
		fprintf(fout, "%d, %d\n", node->value, node->color);
		if (node->r_chlid != NULL){ DF_search(node->r_chlid, fout); }
	}else{
		printf("%d, %d\n", node->value, node->color);
		fprintf(fout, "%d, %d\n", node->value, node->color);
		if (node->r_chlid != NULL){ DF_search(node->r_chlid, fout); }
	}
}