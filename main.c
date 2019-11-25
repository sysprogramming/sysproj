
#include "teamproject.c"
PROJECT a = { 0 };
int main() {
	char op[20];
	clrscr();
	int x, y;
	while (1) {

		show_block(a.ARR[0], a.DOSIZE, DOI);
		show_block(a.ARR[1], a.DOINGSIZE, DOINGI);
		show_block(a.ARR[2], a.DONESIZE, DONEI);

		gotoxy(MENUI, 10);
		printf("명령어를 입력하세요:");
		fgets(op, 19, stdin);

		if (strcmp(op, "a\n") == 0)
			add_block(&a.ARR[0][a.DOSIZE++]);
		else if (strcmp(op, "d\n") == 0) {
			gotoxy(MENUI, 11);
			printf("삭제할 TASK BLOCK의 위치를 입력하세요(x y)");
			scanf("%d %d", &x, &y);
			if (x > 3 || x < 1)
				printf("x는 1,2,3 중 하나여야 합니다.");
			else
			delete_block(a.ARR[x - 1], y - 1, a.DOSIZE--);
		}
		else if (strcmp(op, "m\n") == 0) {
			gotoxy(MENUI, 11);
			printf("이동할 TASK BLOCK의 위치를 입력하세요(x y)");
			scanf("%d %d", &x, &y);
			if (x > 2 || x < 1)
				printf("x는 1,2 중 하나여야 합니다.");
			else
				move_BLOCK(a.ARR[x - 1],a.ARR[x], y - 1,a.DOSIZE--,a.DOINGSIZE++);
		}
		else if (strcmp(op, "q\n") == 0) {
			clrscr();

			break;
		}
		clrscr();
	}
}
