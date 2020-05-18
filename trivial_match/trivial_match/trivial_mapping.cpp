#include <iostream>
#include <fstream>
#include <ctime>
#include <random>
#include <string>
#include <sstream>
#include <algorithm>

// 2015112147 �赵��
using namespace std;

char match[500000]; // ��Ī ��� �����Ǵ� MyDNA�� ���� �迭

string to_str(int x) {
	ostringstream s;
	s << x;

	return s.str();
}


// reference dna�� ����� �Լ�. ref_dna.txt�� �����ȴ�.
void create_ref_dna()
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, 3);
	// ���� ������

	ofstream b("ref_dna.txt");

	int cnt = 0;
	int a; 

	// 500000���� ����
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

	cout << "reference DNA ���� ����! " << endl;
	cout << "size :: " << m.length() << endl;
}


// ���ڰ� ������ ��, �ش� ���ڸ� �ٸ� ���ڷ� �ٲٴ� �Լ�
void return_otherchar(char& ch)
{

	// ���� ����
	srand(time(NULL));

	int dice = rand() % 3; // �ֻ��� ������ dice

	// ���� ���ڰ� A�� ��
	if (ch == 'A')
	{
		if (dice == 0)
			ch = 'T';
		else if (dice == 1)
			ch = 'G';
		else
			ch = 'C';
	}
	// ���� ���ڰ� T�� ��
	else if (ch == 'T')
	{
		if (dice == 0)
			ch = 'A';
		else if (dice == 1)
			ch = 'G';
		else
			ch = 'C';
	}
	// ���� ���ڰ� G�� ��
	else if (ch == 'G')
	{
		if (dice == 0)
			ch = 'T';
		else if (dice == 1)
			ch = 'A';
		else
			ch = 'C';
	}
	// ���� ���ڰ� C�� ��
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

// ���� �迭�� x ��ġ ���� �ߺ��Ǵ� ���Ұ� ���� �� ���� �ٲ���
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

// ũ�Ⱑ x�� ���� �迭�� �ߺ��Ǵ� ���� ���� �������� �Ҵ���
void create_different_index(int*& arr, int x, int k) // k = ����, x = ����.
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


// reference���� x�ۼ�Ʈ �ٸ� my sequence�� ���� �� �װ��� �ɰ� short read�� ����
int create_mysequence_and_shortread(int& k, int& n)
{
	string reference_origin; //���۷��� dna�� ������ ����
	string reference_dna;	// my sequence�� ���� �� ���ڿ�	
	string write_ans;		// short read�� my sequence�� ��𿡼� �Դ��� ���
	string short_read_str;	// short read�� ��� ���ڿ�.

	int a , x;

	ifstream fileread_ref("ref_dna.txt");			// ���۷��� dna open
	ofstream filewrite_seq("my_sequence.txt");		// my sequence txt ����
	ofstream filewrite_short("short_reads.txt");	// short read txt ����
	ofstream filewrite_ans("answer.txt");			// ���� ���� ����
	ofstream filewrite_ans2("answer_inorder.txt");	// ���� ���� �ε��� ������������ ����.



	if (fileread_ref.is_open())
	{
		cout << "���� k, short read����, ���ۼ�Ʈ�� Ʋ���� �� ������ �Է��Ͻÿ� ::";
		cin >> k >> n >> x;
		getchar();



		fileread_ref >> reference_dna;
		reference_origin = reference_dna;

		//////////////////////////////////////////////////////////////////////////////////
		//int nums = (k*x)/100; // ��ü�� x% ��ŭ ���� �߻�. 

		//int* rand_arr = new int[nums];
		/*
		// ���� �߻�
		for (int i = 0; i <= 500000 - k ; i += k)
		{
			create_different_index(rand_arr, nums, k); 
			for (int j = 0; j < nums; j++)
			{
				int c = i + rand_arr[j]; // reference dna�� �ٲ� ��ġ
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
			short_read_str = short_read_str + "T"; // short read ���ڿ� ����.
		}


		filewrite_seq << reference_dna; // ������ my dna txt�� �ű�

		
		fileread_ref.close();
		filewrite_seq.close();
		///////////////////////////////////////////////////////////////////////////////////

		int* short_read_index = new int[n]; // short read�� ��� �Դ���

		int index;
		create_different_index(short_read_index, n, 499999 - k);

		// short read ����
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
		cout << "mysequence �� ref_dna ����ġ :: "<< cnt_mismatch2 << endl;
		filewrite_ans.close();
		filewrite_short.close();
		cout << "���ο� mysequence �� short reads ���� ����" << endl;
		return 1;

	}
	else
	{
		cout << "���� ���� ����" << endl;

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

		cout << "threshold �Է� :: ";

		cin >> threshold;
		getchar();

		cout << "�м� ����." << endl;

		ifstream read_ref_dna("ref_dna.txt"); 
		ifstream read_seq_dna("my_sequence.txt");
		ifstream read_short_read("short_reads.txt");
		ofstream write_generated_seq("gen_seq.txt");

		string my_seq_str;// my dna
		string reference_dna;// reference dna
		string* s_r_array = new string[n]; //short read�� ���� �迭
		string regenerated_sequence; // ������ �м����� ���� dna

		string in_line;
		int cnt = 0;
		int err_cnt = 0;

		read_ref_dna >> reference_dna;	// reference dna
		read_seq_dna >> my_seq_str;		// my dna
		

		// short read �ҷ���
		///////////////////////////////////////////////////////////////////////////////////
		while (getline(read_short_read, in_line))
		{
			s_r_array[cnt++] = in_line;
		}
		cnt = 0;
		///////////////////////////////////////////////////////////////////////////////////



		// ���Ʈ ���� ����
		///////////////////////////////////////////////////////////////////////////////////
		for (int i = 0; i < n; i++)
		{

			cout << n << "���� ���� " << i + 1 << "�� ° �м��� " << endl;


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

		// ������ match �迭. ����
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
		printf("�ɸ� �ð� :: %d��  %.2f �� \n", min, sec);

		cout << "short read�� ���� :: " << k << " , short read�� ���� :: " << n << "��, ��� mismatch :: " << threshold << endl;
		cout << "500000�� �� " << cnt_mismatch << "�� Ʋ���ϴ�." << endl;
		cout << "my sequence���� ���̴� 500000�� �� " << cnt_mismatch2 << "�� Ʋ���ϴ�." << endl;


		write_generated_seq << regenerated_sequence;

		read_ref_dna.close();
		read_short_read.close();
		write_generated_seq.close();
	}
	else
	{
		cout << "���� ����" << endl;
	}
}