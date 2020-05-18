#include <iostream>
#include <fstream>
#include <ctime>
#include <random>
#include <string>
#include <sstream>
#include <algorithm>

// 2015112147 김도훈
using namespace std;

char match[500000]; // 매칭 결과 생성되는 MyDNA를 담을 배열

string to_str(int x) {
	ostringstream s;
	s << x;

	return s.str();
}


// reference dna를 만드는 함수. ref_dna.txt로 생성된다.
void create_ref_dna()
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, 3);
	// 난수 생성기

	ofstream b("ref_dna.txt");

	int cnt = 0;
	int a; 

	// 500000만개 생성
	for (int i = 0; i < 500000; i++)
	{
		a = dis(gen);
		if (a == 0) { b << 'A'; }
		else if (a == 1) { b << 'T'; }
		else if (a == 2) { b << 'G'; }
		else { b << 'C'; }
	}
	b.close();
	string m;
	ifstream check("ref_dna.txt");

	check >> m;

	cout << "reference DNA 생성 성공! " << endl;
	cout << "size :: " << m.length() << endl;
}


// 문자가 들어왔을 때, 해당 문자를 다른 문자로 바꾸는 함수
void return_otherchar(char& ch)
{

	// 난수 생성
	srand(time(NULL));

	int dice = rand() % 3; // 주사위 돌리듯 dice

	// 들어온 문자가 A일 때
	if (ch == 'A')
	{
		if (dice == 0)
			ch = 'T';
		else if (dice == 1)
			ch = 'G';
		else
			ch = 'C';
	}
	// 들어온 문자가 T일 때
	else if (ch == 'T')
	{
		if (dice == 0)
			ch = 'A';
		else if (dice == 1)
			ch = 'G';
		else
			ch = 'C';
	}
	// 들어온 문자가 G일 때
	else if (ch == 'G')
	{
		if (dice == 0)
			ch = 'T';
		else if (dice == 1)
			ch = 'A';
		else
			ch = 'C';
	}
	// 들어온 문자가 C일 때
	else
	{
		if (dice == 0)
			ch = 'T';
		else if (dice == 1)
			ch = 'G';
		else
			ch = 'A';
	}
}

// 정수 배열의 x 위치 까지 중복되는 원소가 없을 때 까지 바꿔줌
bool chk_repeat(int*& arr, int x)
{
	if (x != 0)
	{
		for (int i = 0; i < x; i++)
		{
			if (arr[i] == arr[x])
			{
				return false;
			}
		}
		return true;
	}
	else { return true; }
}

// 크기가 x인 정수 배열에 중복되는 값이 없는 난수들을 할당함
void create_different_index(int*& arr, int x, int k) // k = 범위, x = 갯수.
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, k-1);

	for (int i = 0; i < x; i++)
	{
		arr[i] = dis(gen);
		while (!chk_repeat(arr, i)) 
		{
			arr[i] = rand() % k;
		}
	}
}


// reference에서 x퍼센트 다른 my sequence를 만든 뒤 그것을 쪼개 short read로 만듦
int create_mysequence_and_shortread(int& k, int& n)
{
	string reference_origin; //레퍼런스 dna의 원본을 저장
	string reference_dna;	// my sequence로 조작 될 문자열	
	string write_ans;		// short read가 my sequence의 어디에서 왔는지 기록
	string short_read_str;	// short read가 담길 문자열.

	int a , x;

	ifstream fileread_ref("ref_dna.txt");			// 레퍼런스 dna open
	ofstream filewrite_seq("my_sequence.txt");		// my sequence txt 생성
	ofstream filewrite_short("short_reads.txt");	// short read txt 생성
	ofstream filewrite_ans("answer.txt");			// 정답 파일 생성
	ofstream filewrite_ans2("answer_inorder.txt");	// 정답 파일 인덱스 오름차순으로 정리.



	if (fileread_ref.is_open())
	{
		cout << "길이 k, short read갯수, 몇퍼센트를 틀리게 할 것인지 입력하시오 ::";
		cin >> k >> n >> x;
		getchar();



		fileread_ref >> reference_dna;
		reference_origin = reference_dna;

		//////////////////////////////////////////////////////////////////////////////////
		//int nums = (k*x)/100; // 전체의 x% 만큼 스닙 발생. 

		//int* rand_arr = new int[nums];
		/*
		// 스닙 발생
		for (int i = 0; i <= 500000 - k ; i += k)
		{
			create_different_index(rand_arr, nums, k); 
			for (int j = 0; j < nums; j++)
			{
				int c = i + rand_arr[j]; // reference dna가 바뀔 위치
				return_otherchar(reference_dna[c]);
			}
			
		}*/
		///////////////////////////////////////////////////////////////////////////////////


		///////////////////////////////////////////////////////////////////////////////////
		int nums = 5000 * x;
		int* rand_arr = new int[nums];
		int c;
		create_different_index(rand_arr, nums, 500000);
		for (int i = 0; i < nums ; i++)
		{
			c = rand_arr[i];
			return_otherchar(reference_dna[c]);
		}


		for (int i = 0; i < k; i++)
		{
			short_read_str = short_read_str + "T"; // short read 문자열 생성.
		}


		filewrite_seq << reference_dna; // 생성된 my dna txt로 옮김

		
		fileread_ref.close();
		filewrite_seq.close();
		///////////////////////////////////////////////////////////////////////////////////

		int* short_read_index = new int[n]; // short read가 어디서 왔는지

		int index;
		create_different_index(short_read_index, n, 499999 - k);

		// short read 생성
		for (int i = 0; i < n; i++)
		{
			index = short_read_index[i];
			for (int j = 0; j < k; j++)
			{
				short_read_str[j] = reference_dna[index++];
			}
			filewrite_short << short_read_str + "\n";
		}

		int cnt_mismatch2 = 0;

		for (int i = 0; i < 500000; i++)
		{
			if (reference_dna[i] != reference_origin[i])
			{
				(cnt_mismatch2)++;
			}
		}

		
		///////////////////////////////////////////////////////////////////////////
		for (int i = 0; i < n; i++)
		{
			filewrite_ans << to_str(short_read_index[i]) + "\n";
		}

		sort(short_read_index, short_read_index + n);

		for (int i = 0; i < n; i++)
		{
			filewrite_ans2 << to_str(short_read_index[i]) + "\n";
		}
		///////////////////////////////////////////////////////////////////////////
		cout << "mysequence 와 ref_dna 불일치 :: "<< cnt_mismatch2 << endl;
		filewrite_ans.close();
		filewrite_short.close();
		cout << "새로운 mysequence 와 short reads 생성 성공" << endl;
		return 1;

	}
	else
	{
		cout << "파일 열기 실패" << endl;

		return 0;
	}
}



int BF_shortread(string ref_d, string short_rd,int k_size ,int threshold , int start_point)
{
	int i;
	int cnt = 0;
	for (i = start_point; i < 500000 - k_size; i++)
	{
		for (int j = 0; j < k_size; j++)
		{
			if ((ref_d[i + j] != short_rd[j]))
			{
				cnt++;
			}
		}
		if ((cnt < threshold))
		{
			return i;
		}
		else
		{
			cnt = 0;
		}
	}
	if (cnt > threshold)
	{
		return -1;
	}
}


int main()
{

	create_ref_dna();

	clock_t clk_start = clock();
	int k, n;
	if (create_mysequence_and_shortread(k,n))
	{
		int threshold;

		cout << "threshold 입력 :: ";

		cin >> threshold;
		getchar();

		cout << "분석 시작." << endl;

		ifstream read_ref_dna("ref_dna.txt"); 
		ifstream read_seq_dna("my_sequence.txt");
		ifstream read_short_read("short_reads.txt");
		ofstream write_generated_seq("gen_seq.txt");

		string my_seq_str;// my dna
		string reference_dna;// reference dna
		string* s_r_array = new string[n]; //short read를 담을 배열
		string regenerated_sequence; // 숏리드 분석으로 생긴 dna

		string in_line;
		int cnt = 0;
		int err_cnt = 0;

		read_ref_dna >> reference_dna;	// reference dna
		read_seq_dna >> my_seq_str;		// my dna
		

		// short read 불러옴
		///////////////////////////////////////////////////////////////////////////////////
		while (getline(read_short_read, in_line))
		{
			s_r_array[cnt++] = in_line;
		}
		cnt = 0;
		///////////////////////////////////////////////////////////////////////////////////



		// 브루트 포스 진행
		///////////////////////////////////////////////////////////////////////////////////
		for (int i = 0; i < n; i++)
		{

			cout << n << "개중 현재 " << i + 1 << "번 째 분석중 " << endl;


			int index = BF_shortread(reference_dna, s_r_array[i], k ,threshold, 0);
			
			if (index != -1)
			{
				for (int j = 0; j < k; j++)
				{
					match[index + j] = s_r_array[i][j];
				}
			}
		}
		///////////////////////////////////////////////////////////////////////////////////

		// 생성된 match 배열. 정리
		for (int i = 0; i < 500000; i++)
		{
			if (match[i] == NULL)
			{
				match[i] = 'N';
			}
		}

		regenerated_sequence = match;

		int cnt_mismatch = 0;
		int cnt_mismatch2 = 0;

		for (int i = 0; i < 500000; i++)
		{
			if (reference_dna[i] != match[i])
			{
				cnt_mismatch++;
			}
		}

		for (int i = 0; i < 500000; i++)
		{
			if (my_seq_str[i] != match[i])
			{
				(cnt_mismatch2)++;
			}
		}

		double due_time = ((float)clock() - clk_start) / CLOCKS_PER_SEC;
		int min = due_time / 60;
		double sec = due_time - 60*min;
		printf("걸린 시간 :: %d분  %.2f 초 \n", min, sec);

		cout << "short read의 길이 :: " << k << " , short read의 갯수 :: " << n << "개, 허용 mismatch :: " << threshold << endl;
		cout << "500000개 중 " << cnt_mismatch << "개 틀립니다." << endl;
		cout << "my sequence와의 차이는 500000개 중 " << cnt_mismatch2 << "개 틀립니다." << endl;


		write_generated_seq << regenerated_sequence;

		read_ref_dna.close();
		read_short_read.close();
		write_generated_seq.close();
	}
	else
	{
		cout << "생성 실패" << endl;
	}
}