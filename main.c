
#include "teamproject.c"
PROJECT a = { 0 };
int main() {
	char op[20];
	clrscr();
	int x, y;
	while (1) {

		show_block(a.ARR[0], a.SIZE[0], DOI);
		show_block(a.ARR[1], a.SIZE[1], DOINGI);
		show_block(a.ARR[2], a.SIZE[2], DONEI);

		gotoxy(MENUI, 10);
		printf("Select the operation:");
		fgets(op, 19, stdin);

		if (strcmp(op, "a\n") == 0)
			add_block(&a.ARR[0][a.SIZE[0]++]);
		else if (strcmp(op, "d\n") == 0) {
			gotoxy(MENUI, 11);
			printf("Type the data blocks position you want to delete(x y):");
			scanf("%d %d", &x, &y);
			if (x > 3 || x < 1)
				printf("Wrong position");
			else
			delete_block(a.ARR[x - 1], y - 1, a.SIZE[x-1]--);
		}
		else if (strcmp(op, "m\n") == 0) {
			gotoxy(MENUI, 11);
			printf("Type the data blocks position you want to move(x y)");
			scanf("%d %d", &x, &y);
			if (x > 2 || x < 1)
				printf("Wrong position");
			else
				move_BLOCK(a.ARR[x - 1],a.ARR[x], y - 1,a.SIZE[x-1]--,a.SIZE[x]++);
		}
		else if (strcmp(op, "q\n") == 0) {
			clrscr();

			break;
		}
		clrscr();
	}
}
