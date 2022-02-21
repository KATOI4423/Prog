#include<iostream>
#include<fstream>
#include<vector>
#include<cstdio>
#include<conio.h>

#define BACKSPACE 8 /* KeyCode of Backspace */

using namespace std;

const int MAX_LEVEL = 9; /* MaxLevel is Now +9 */
int MAX_NUM;/* This is The num of elements of vector */

class CNT {
private: 
	unsigned long cnt;

public:
	void up(unsigned long n) {/* cnt +n */
		cnt+=n;
	}
	void down(unsigned long n) {/* cnt -n */
		cnt-=n;
	}
	void print() {/* write down the cnt */
		printf("%lu", cnt);
	}
	void renew(unsigned long n) {/* renew to n */
		cnt = n;
	}
	unsigned long val(void) {
		return cnt;
	}
};

void MAKE_FILE(string);
void ERROR_AGAIN_INPUT(void);
void SAVE(vector<vector<CNT> >, CNT, string);

int main() {
	unsigned long start_lvl = 0;
	char check = '\0';
	unsigned long bun, log = 0;
	int bui;
	string FName;
	CNT n;/* Level */
	CNT times;/* cnt of How many challenge */
	
	/* Checking the Num of Slot */
	cout << "\nSlot? [0, 1, 2]" << endl;
	cout << "if you test -> T" << endl;
	do {
		cin >> check;
		if (check == '0') {
			FName = "SLOT_0.txt";
			MAX_NUM = 3;
		}
		else if (check == '1') {
			FName = "SLOT_1.txt";
			MAX_NUM = 4;
		}
		else if (check == '2') {
			FName = "SLOT_2.txt";
			MAX_NUM = 5;
		}
		else if (check == 't' || check == 'T') {
			FName = "TEST.txt";
			MAX_NUM = 5;
		}
		else {
			ERROR_AGAIN_INPUT();
			continue;
		}
	} while (false);

	vector<vector<CNT> >cnt(MAX_LEVEL, vector<CNT>(MAX_NUM));
	/*
	 * initialise by 0
	 * cnt[n][0] -> success
	 * cnt[n][1] -> failure (not down)
	 * cnt[n][2] -> failure ( 1 down )
	 * cnt[n][3] -> failure ( 2 down )
	 * cnt[n][4] -> failure ( 3 down )
	 */

	 /* Loading */
	FILE* fp = fopen(FName.c_str(), "r");
	if (fp == NULL) {
		cerr << "ERROR : FILE OPEN" << endl;
		cerr << "We'll make Data file" << endl;

		MAKE_FILE(FName);

		cerr << "Success\n" << endl;
		fclose(fp);
		fp = fopen(FName.c_str(), "r");
	}
	fscanf(fp, "%lu", &bun);
	log = bun;
	times.renew(log);
	for (int i = 0; i < MAX_LEVEL; i++) {
		for (int j = 0; j < MAX_NUM; j++) {
			fscanf(fp, "%lu", &bun);
			log = bun;
			cnt[i][j].renew(log);
		}
	}
	fclose(fp);

	/* Checking Level when you start */
	cout << "\nNow Level is ?" << endl;
	do {
		cin >> start_lvl;

	} while (!(0 <= start_lvl && start_lvl < 9));

	n.renew(start_lvl);

	/* Checking whether use high-mithril or not */
	cout << "\nDo you use any Item ?\n" << endl;
	do {
		cout << "if use High-Mithril -> input M" << endl;
		cout << "if use HIhiyarokane -> input H" << endl;
		cout << "if   Don't   use    -> input N" << endl;
		cout << "if delete this data -> input D\n" << endl;

		cin >> check;

		if (check == 'm' || check == 'M') {
			start_lvl = 6;
		}
		else if (check == 'h' || check == 'H') {
			start_lvl = 7;
		}
		else if (check == 'n' || check == 'N') {
			start_lvl = 0;
		}
		/* DELETE */
		else if (check == 'd' || check == 'D') {
			cout << "DELETE DATA ? [Y or N]" << endl;
			cin >> check;
			if (check == 'y' || check == 'Y') {
				MAKE_FILE(FName);

				cout << "Succecc DELETE" << endl;
				cout << "Check the Data" << endl;
				return 0;
			}
		}
		else {
			check = '\0';
		}
	} while (check=='\0');

	/* Introduce */
	cout << "\nInput as follows\n"
		<< "Succecc	           -> s\n"
		<< "Failure (not down) -> q\n"
		<< "Failure ( 1 down ) -> 1\n"
		<< "Failure ( 2 down ) -> 2\n"
		<< "Failure ( 3 down ) -> 3\n"
		<< "Modificate         -> Back space\n"
		<< "END -> @\n" << endl;

	
	do {/* Count */
		check = getch();
		cout << check;

		/* Level check */
		if (n.val() == 9) {
			n.renew(start_lvl);
		}

		/* inut */
		if (check == '@') {
			break;
		}
		else if (check == 's') {/* succecc */
			cout << " : " << n.val() << " -> ";
			bui = 0;
			bun = n.val();
			cnt[bun][bui].up(1);
			n.up(1);
			times.up(1);
			cout << n.val() << endl;
		}
		else if (check == 'q') {/* not down */
			cout << " : " << n.val() << " -> ";
			bui = 1;
			bun = n.val();
			cnt[bun][bui].up(1);
			times.up(1);
			cout << n.val() << endl;
		}
		else if (check == BACKSPACE) {/* misstake */
			cout << " : " << n.val() << " -> ";
			times.down(1);
			cnt[n.val()][bui].down(1);
			n.renew(bun);
			cout << n.val() << endl;
		}
		else {

			int num = (int)(check - '0');

			if ((num > 0) && (num < (MAX_NUM - 1))) {/* down just num */
				cout << " : " << n.val() << " -> ";
				bui = num + 1;
				bun = n.val();
				cnt[bun][bui].up(1);
				n.down(num);
				if (n.val() > 9) {
					n.renew(0);
				}
				times.up(1);
				cout << n.val() << endl;
			}
			else {/* others */
				ERROR_AGAIN_INPUT();
			}
		}
		if (times.val() % 10 == 1) {
			SAVE(cnt, times, FName);
		}
		
	} while (check != '@');
	SAVE(cnt, times, FName);
	
	cout << "\nSuccecc! Have A Good IRUNA Life..." << endl;
	check = getch();
	return 0;
}

void ERROR_AGAIN_INPUT(void) {
	cerr << "\nYou input Strange key\n"
		<< "Input again" << endl;
}

void SAVE(vector<vector<CNT> >cnt,CNT times, string FName) {
	cout << "\nNow Calculating Provavility..." << endl;
	
	double P[MAX_LEVEL][MAX_NUM];
	for (int i = 0; i < MAX_LEVEL; i++) {
		unsigned long SUM = 0;
		for (int j = 0; j < MAX_NUM; j++) {
			SUM += cnt[i][j].val();
		}
		if (SUM == 0) {
			for (int j = 0; j < MAX_NUM; j++) {
				P[i][j] = 0;
			}
		}
		else {
			for (int j = 0; j < MAX_NUM; j++) {
				P[i][j] = cnt[i][j].val();
				P[i][j] *= 100 / SUM;
			}
		}
	}

	cout << "Now Outputing Data" << endl;

	const char* fname = FName.c_str();
	FILE* fp = fopen(fname, "r+");
	if (fp==NULL) {
		cerr << "Failure Open FILE" << endl;
		exit(1);
	}
	fprintf(fp, "%lu\n", times.val());
	int N = MAX_NUM - 1;
	for (int i = 0; i < MAX_LEVEL; i++) {
		int j;
		for (j = 0; j < N; j++) {
			fprintf(fp, "%lu ", cnt[i][j].val());
		}
		fprintf(fp, "%lu\n", cnt[i][j].val());
	}
	fprintf(fp, "\n--- Probability ---\n");
	for (int i = 0; i < MAX_LEVEL; i++) {
		int j;
		fprintf(fp, "Lv%d : ", i);
		for (j = 0; j < N; j++) {
			fprintf(fp,"%6.4f ", P[i][j]);
		}
		fprintf(fp,"%6.4f\n", P[i][j]);
	}
	fclose(fp);
	cout << "Success!\n" << endl;
}

void MAKE_FILE(string FName) {
	int N = MAX_NUM - 1;
	FILE* fp = fopen(FName.c_str(), "w");
	if (fp == NULL) {
		cerr << "ERROR : MAKE FILE" << endl;
	}
	fprintf(fp, "0\n");
	for (int i = 0; i < MAX_LEVEL; i++) {
		for (int j = 0; j < N; j++) {
			fprintf(fp, "0 ");
		}
		fprintf(fp, "0\n");
	}
	fclose(fp);
}