#include<iostream>
#include<conio.h>
#include<fstream>
#include<string>
#include<string.h>
#include"Console.h"
#pragma warning(disable:4996)
using namespace std;

//Tra cứu từ điển : tổ chức từ điển theo cấu trúc danh sách liên kết kép.Mỗi từ gồm có các
//thông tin : Từ, loại từ, các nghĩa việt(theo cấu trúc danh sách liên kết đơn), các ví dụ(theo
//cấu trúc mảng con trỏ, tối đa 5 ví dụ).
//Chương trình có các chức năng : nhập từ mới, hiệu chỉnh từ, xóa từ, tra từ theo 2 cách : nhập
//đến đâu thì thanh sáng sẽ nhảy ngay đến từ đầu tiên có các ký tự đầu trùng với các ký tự vừa
//nhập, hoặc dùng thanh sáng di chuyển đến từ cần tra, ghi từ vào file, load từ vào danh sách
//liên kết kép…
//Lưu ý : -Danh sách các từ được tổ chức theo cấu trúc mảng băm
//- Các thuật toán xử lý trên từ điển đều thực hiện trong bộ nhớ trong.File chỉ là
//nơi lưu các từ khi có sự thay đổi trên từ điển trong bộ nhớ trong.

#define widthConSoLe 120
#define heightConSoLe 900
#define soChuCai 27
#define maxVidu 6

short STrang = 1;
short SViTriChon = 1;
short STongMuc = 3;
bool isSearch = false; // Biến này là biến xác định có nhập bằng chữ để tìm hay không
// false nếu chỉ phím mũi tên đi lên, đi xuống
// true nếu nhập tìm từ bằng chữ cái

// phần nghĩa
struct NodeNghia // Nghĩa của 1 từ
{
	string tenNghia;
	NodeNghia *pNext;
};
typedef struct NodeNghia NODENGHIA;
NODENGHIA* khoiTaoNodeNghia(string x)	// Hàm tạo 1 nghĩa
{
	NODENGHIA *p = new NODENGHIA; // tạo ra 1 ô nhớ chứa nghĩa
	p->tenNghia = x;		// truyền nghĩa vào trong ô nhớ
	p->pNext = NULL;	// chưa kết nối đến nghĩa nào hết
	return p;	// Trả về con trỏ p ( trả về địa chỉ ô nhớ cấp phát)
}
void themNodeNghia(NODENGHIA *&pHead, NODENGHIA *p) // Thêm vào cuối
{
	if (pHead == NULL)
	{
		pHead = p;
	}
	else
	{
		for (NODENGHIA* k = pHead;; k = k->pNext)
		{
			if (k->pNext == NULL)
			{
				k->pNext = p;
				break;
			}
		}
	}
}
void xuatDSNghia(NODENGHIA *pHead)// hàm xuất các nghĩa
{
	for (NODENGHIA *k = pHead; k != NULL; k = k->pNext)
	{
		cout << k->tenNghia;
		if (k->pNext != NULL)
		{
			cout << ", ";
		}
	}
}
void docDSNghia(ifstream& filein, NODENGHIA *&pHead)
{
	string nghia;
	while (!filein.eof())
	{
		getline(filein, nghia, '/');
		if (nghia == ".")
		{
			return;
		}
		themNodeNghia(pHead, khoiTaoNodeNghia(nghia));
	}
}
void ghiDSNghia(ofstream& fileout, NODENGHIA *&pHead)
{
	for (NODENGHIA *k = pHead; k != NULL; k = k->pNext)
	{
		fileout << k->tenNghia << "/";
	}
	fileout << "./";
}
void giaiPhongDSNghia(NODENGHIA *&pHead)
{
	NODENGHIA *k = NULL;
	while (pHead != NULL)
	{
		k = pHead;
		pHead = pHead->pNext;
		delete k;
	}
}

// phần ví dụ
struct ListViDu
{
	string *viDu[maxVidu];
	short n = 1;// n là số lượng ví dụ có trong từ
};
typedef struct ListViDu LISTVIDU;
void xuatDSVidu(LISTVIDU lvd)
{
	short vt = 26;
	cout << endl;
	for (short i = 1; i < lvd.n; i++)
	{
		gotoXY(59, vt++);
		cout << "+ " << *lvd.viDu[i];
	}
}
void docDSViDu(ifstream& filein, LISTVIDU &lvd)
{
	string viDu;
	for (; lvd.n < maxVidu; lvd.n++)
	{
		getline(filein, viDu, '/');
		if (viDu == "")
		{
			return;
		}
		else
		{
			lvd.viDu[lvd.n] = new string{ viDu };
		}
	}
}
void ghiDSViDu(ofstream& fileout, LISTVIDU &lvd)
{
	short i;
	for (i = 1; i < lvd.n; i++)
	{
		fileout << *lvd.viDu[i] << "/";
	}
	if (lvd.n != maxVidu)
	{
		fileout << "/";
	}
}
void giaiPhongDSViDu(LISTVIDU &lvd)
{
	for (short i = 1; i < lvd.n; i++)
	{
		delete lvd.viDu[i];
	}
}

// phần kiểu dữ liệu từ
struct Tu // Thông tin 1 từ
{
	string ten;// Tên từ
	string loaiTu;// loại từ
	NODENGHIA *pHead = NULL;// các nghĩa việt(theo cấu trúc danh sách liên kết đơn)
	LISTVIDU lvd; //các ví dụ(theo cấu trúc mảng con trỏ, tối đa 5 ví dụ) .
};
typedef struct Tu TU;
void xuat1Tu(TU x)
{
	gotoXY(50, 22);
	cout << "- TEN TU:  " << x.ten;
	gotoXY(50, 23);
	cout << "- LOAI TU: ";
	gotoXY(61, 23);
	cout << x.loaiTu;
	gotoXY(50, 24);
	cout << "- NGHIA: ";
	gotoXY(61, 24);
	xuatDSNghia(x.pHead);
	gotoXY(50, 25);
	cout << "- VI DU: ";
	xuatDSVidu(x.lvd);
}
void doc1Tu(ifstream& filein, TU &x)// đọc 1 dòng
{
	getline(filein, x.ten, '/');
	getline(filein, x.loaiTu, '/');
	docDSNghia(filein, x.pHead);
	docDSViDu(filein, x.lvd);
	char a;
	filein.get(a);// đọc kí tự xuống dòng ở cuối mỗi từ
}
void ghi1Tu(ofstream& fileout, TU &x)// đọc 1 dòng
{
	fileout << x.ten << "/" << x.loaiTu << "/";
	ghiDSNghia(fileout, x.pHead);
	ghiDSViDu(fileout, x.lvd);
	fileout << '\n';
}
void giaiPhong1Tu(TU &x)
{
	giaiPhongDSNghia(x.pHead);
	giaiPhongDSViDu(x.lvd);
}

// phần Node Từ
struct NodeTu // Node 1 từ trong dslk đôi
{
	TU data;
	NodeTu *pNext;
	NodeTu *pRev;
};
typedef struct NodeTu NODETU;
NODETU* khoiTaoNodeTu(Tu x)
{
	NODETU* p = new NODETU;
	p->data = x;
	p->pNext = NULL;
	p->pRev = NULL;
	return p;
}

// Phần 1 dãy node từ
struct Day// 1 dãy
{
	NODETU* pHead = NULL;
	NODETU* pTail = NULL;
};
typedef struct Day DAY;
void ThemNodeTuVao1Day(DAY &d, NODETU *p)// Thêm vào cuối
{
	if (d.pHead == NULL)
	{
		d.pHead = d.pTail = p;
	}
	else
	{
		p->pRev = d.pTail;
		d.pTail->pNext = p;
		d.pTail = p;
	}
}
void xuat1Day(DAY d)// hàm xuất các từ trong dãy
{
	for (NODETU *k = d.pHead; k != NULL; k = k->pNext)
	{
		xuat1Tu(k->data);
	}
}
void xoaDauDay(DAY &d)
{
	if (d.pHead == NULL && d.pTail == NULL)	// Không còn phần tử nào
	{
		return;
	}
	else if (d.pHead->pNext == NULL)	// Còn 1 phần tử
	{
		NODETU* k = d.pHead;
		d.pHead = NULL;
		d.pTail = NULL;
		delete k;
		return;
	}
	else
	{
		NODETU* k = d.pHead;
		d.pHead = d.pHead->pNext;
		d.pHead->pRev = NULL;
		delete k;
	}
}
void xoaCuoiDay(DAY &d)
{
	if (d.pHead == NULL && d.pTail == NULL)	// Không còn phần tử nào
	{
		return;
	}
	else if (d.pTail->pRev == NULL)	// Còn 1 phần tử
	{
		NODETU* k = d.pTail;
		d.pHead = NULL;
		d.pTail = NULL;
		delete k;
		return;
	}
	else
	{
		NODETU* k = d.pTail;
		d.pTail = d.pTail->pRev;
		d.pTail->pNext = NULL;
		delete k;
	}
}
void xoa_Node_Tu_vao_vtri_bat_ki(DAY &d, int vt)
{
	int n = 0;
	for (NODETU *k = d.pHead; k != NULL; k = k->pNext)
	{
		n++;
	}
	if (vt == 0)
	{
		xoaDauDay(d);
	}
	else if (vt == n - 1)
	{
		xoaCuoiDay(d);
	}
	else
	{
		int dem = 0;
		for (NODETU *k = d.pHead; k != NULL; k = k->pNext)
		{
			if (dem + 1 == vt)
			{
				NODETU* g = k->pNext;
				k->pNext = k->pNext->pNext;
				delete g;
				return;
			}
			dem++;
		}
	}
}
void ghiFileTuDay(ofstream& fileout, DAY &d)
{
	for (NODETU *k = d.pHead; k != NULL; k = k->pNext)
	{
		ghi1Tu(fileout, k->data);
	}
}
void giaiPhong1Day(DAY &d)
{
	NODETU *k = NULL;
	while (d.pHead != NULL)
	{
		k = d.pHead;
		giaiPhong1Tu(k->data);
		d.pHead = d.pHead->pNext;
		delete k;
	}
}

// phần từ điển bao gồm các dãy
struct TuDien// Danh sách các dãy (mảng con trỏ các dãy)
{
	DAY dSDay[soChuCai];
};
typedef struct TuDien TUDIEN;
void themTuVaoTuDien(TUDIEN &td, TU str)// Hàm thêm 1 từ vào từ điển
{
	char x = str.ten[0];
	short vt = (int)x - 96; // vị trí bỏ vào mảng băm với chữ thường
	ThemNodeTuVao1Day(td.dSDay[vt], khoiTaoNodeTu(str));// bỏ tương ứng vào dãy
}
void xuatTuDien(TUDIEN td)
{
	for (short i = 1; i< soChuCai; i++)
	{
		xuat1Day(td.dSDay[i]);
	}
}
void docFileVaoTuDien(TUDIEN &td)
{
	ifstream filein;
	filein.open("filedoctudien.txt", ios_base::in);
	while (!filein.eof())
	{
		TU x;
		doc1Tu(filein, x);
		themTuVaoTuDien(td, x);
	}
	filein.close();
}
void ghiFileTuTuDien(TUDIEN &td)
{
	ofstream fileout;
	fileout.open("fileghitutudien.txt", ios_base::out);
	for (short i = 1; i< soChuCai; i++)
	{
		ghiFileTuDay(fileout, td.dSDay[i]);
	}
	fileout.close();
}
short soLuongTu(TUDIEN &td)
{
	short dem = 0;
	for (short i = 1; i< soChuCai; i++)
	{
		for (NODETU *k = td.dSDay[i].pHead; k != NULL; k = k->pNext)
		{
			dem++;
		}
	}
	return dem;
}
void giaiPhongTuDien(TUDIEN &td)
{
	for (short i = 1; i < soChuCai; i++)
	{
		giaiPhong1Day(td.dSDay[i]);
	}
}

//  ================ PHẦN DESIGN =================

// Vẽ các khung màu cam
void veKhung(short x, short y, short width, short height)
{
	char a, b, c, d, e, f; //Ki tu dac biet 
	a = 201;// góc trái
	b = 205;// ngang
	c = 187;// góc phải
	d = 186;// dọc
	e = 200;// góc trái dưới
	f = 188;// góc dưới phải

	setColor(6);// màu cam

	gotoXY(x, y);// chạy tới góc trên trái
	cout << a; // in goc tren trai
	cout << string(width - 1, b); // In ra hàng ngang trên

	gotoXY(x + width, y); // chạy tới góc trên phải
	cout << c;// in goc tren phai

	// in hàng dọc bên trái
	for (short i = 1; i < height; i++)
	{
		gotoXY(x, y + i);
		cout << d;
	}

	// in hàng dọc bên phải
	for (short i = 1; i < height; i++)
	{
		gotoXY(x + width, y + i);
		cout << d;
	}

	gotoXY(x, y + height); // chạy tới góc dưới trái
	cout << e;// in goc dưới trai

	cout << string(width - 1, b); // In ra hàng ngang dưới

	gotoXY(x + width, y + height); // chạy tới góc dưới phải
	cout << f;// in goc dưới phải

	setColor(15);// màu trắng
}

// phần design màn hình 1 và 2
// khung và các ngăn màu cam
void veKhung12()
{
	// tọa độ (7,0), chiều ngang 120 - 20 = 100, chiều dọc 40 - 2 = 38
	short x = 7, y = 0, width = 120 - 20, height = 40 - 2;
	char b = 205;// ngang

	veKhung(x, y, width, height);

	setColor(6);// màu cam
	// In ra ngang giữa (Chứa từ điển)
	gotoXY(x + 1, y + 8);
	cout << string(width - 1, b);

	// In ra ngang giữa (Chứa phím)
	gotoXY(x + 1, y + 36);
	cout << string(width - 1, b);

	setColor(15);// màu trắng
}
void veNganCacPhimMH12()
{
	setColor(6);// màu cam

	// tọa độ (7,0), chiều ngang 120 - 20 = 100, chiều dọc 40 - 2 = 38
	short x = 7, y = 0;

	char d = 186;// dọc

	// In ra dọc ngăn Enter với các phím mũi tên
	gotoXY(x + 7 + 20, y + 35 + 2);
	cout << d;

	// In ra dọc ngăn các phím mũi tên với ESC
	gotoXY(x + 7 + 65, y + 35 + 2);
	cout << d;


	setColor(15);// màu trắng
}
//lệnh màu trắng
void veLenhMH12(short mau, string esc)
{
	setColor(mau);//chữ xanh lá cây
	//màu trắng
	gotoXY(15, 37);
	cout << "CHON: ENTER";

	gotoXY(44, 37);
	cout << "DI CHUYEN: CAC PHIM MUI TEN";

	gotoXY(89, 37);
	cout << esc;
	setColor(16);//chữ trắng
}

// Phần màn hình 1

void veTieuDeMH1(short mau)
{
	setColor(mau);// màu xanh lá = 10
	ifstream filein;
	filein.open("vehinh.txt", ios_base::in);
	short i = 1;
	string a;
	while (!filein.eof())
	{
		getline(filein, a);
		gotoXY(25, i++);
		cout << a << endl;
	}
	filein.close();
	setColor(15);// màu trắng
}
void veMenuMH1(short SInDex)// vị trí đang chọn
{
	// cập nhật lại vị trí đang chọn
	SViTriChon = SInDex;
	STongMuc = 3;

	// vẽ menu
	LPSTR STRTextMenuChinh = "  TIM KIEM   ";
	setColorBGTextXY((120 / 2) - (strlen(STRTextMenuChinh) / 2) - 3, 9, 15, (SInDex == 1) ? 2 : 0, STRTextMenuChinh);

	STRTextMenuChinh = "   THEM TU   ";
	setColorBGTextXY((120 / 2) - (strlen(STRTextMenuChinh) / 2) - 3, 10, 15, (SInDex == 2) ? 2 : 0, STRTextMenuChinh);

	STRTextMenuChinh = "    THOAT    ";
	setColorBGTextXY((120 / 2) - (strlen(STRTextMenuChinh) / 2) - 3, 11, 15, (SInDex == 3) ? 2 : 0, STRTextMenuChinh);
}
void manHinh1()
{
	veKhung12();
	veNganCacPhimMH12();
	veTieuDeMH1(10);// màu xanh lá
	veMenuMH1(1);
	veLenhMH12(10, "THOAT: ESC");// màu xanh lá
	Cursor(false);// ẩn con trỏ
}

//  ================ PHẦN THÊM TỪ MỚI VÀO TỪ ĐIỂN =================

// Các biến trong xử lý thêm từ
struct xuLyThemTu
{
	short Ok = 0;
	short nhapLai = 0;
	short VT = 1;
	bool isThemTu = 0;
	string tu;
	TU them;
} q;
// Phần xử lý thêm từ mới vào từ điển
bool IsChuoiKiTu(string ten)// kiểm tra có phải 1 chuỗi hoàn toàn (chỉ có chữ)
{
	for (short i = 0; i < ten.length(); i++)
	{
		if (!((ten[i] >= 'a' && ten[i] <= 'z') || (ten[i] >= 'A' && ten[i] <= 'Z')))
		{
			return 0;
		}
	}
	return 1;
}
//Lúc này từ đã đúng cú pháp
bool IsTuCoTonTaiTrongTuDien(TUDIEN &td, string ten)	// kiểm tra tên từ có tồn tại sẵn trong từ điển rồi hay chưa
{
	strlwr((char*)ten.c_str());// chuyển từ này về dạng viết thường vĩnh viễn
	char x = ten[0];
	short vt = x - 96; // vị trí bỏ vào mảng băm với chữ thường
	for (NODETU *k = td.dSDay[vt].pHead; k != NULL; k = k->pNext)
	{
		if (k->data.ten == ten)
		{
			return 1;
		}
	}
	return 0;
}
// nhập tên loại từ, vẽ lại khung hiển thị sai  và lệnh tô đỏ
void xuLyTenTu(TUDIEN &td, string &tentu)
{
	gotoXY(14, 37);
	for (int i = 1; i <= 15; i++)
	{
		cout << " ";
	}
	// xóa chữ NHAP LAI: ENTER
	setColor(10);//chữ xanh lá cây
	gotoXY(15, 37);
	cout << "CHON: ENTER";
	gotoXY(83, 37);
	setColor(10);
	cout << "TRO VE TRANG CHU: ESC";

	veKhung(37, 12, 40, 2);
	gotoXY((120 / 2) - 20, 13);
	cout << "NHAP TU: ";
	Cursor(true);// mở con trỏ
	getline(cin, tentu);
	strlwr((char*)tentu.c_str());// chuyển từ về chữ thường vĩnh viễn

	if (tentu == "")// bỏ trống
	{
		veKhung(78, 12, 25, 2);
		gotoXY(79, 13);
		setColor(4);//chữ đỏ

		cout << "XXX: KHONG DUOC BO TRONG";
		Cursor(false);// đóng con trỏ
		gotoXY(14, 37);
		setColor(10);//chữ xanh lá cây
		cout << "NHAP LAI: ENTER";
		q.nhapLai = 1;
		setColor(15);// màu trắng
		return;
	}
	else if (!IsChuoiKiTu(tentu))// kiểm tra cú pháp từ
	{
		veKhung(78, 12, 25, 2);
		gotoXY(82, 13);
		setColor(4);//chữ đỏ

		cout << "XXX: SAI CU PHAP  ";
		Cursor(false);// đóng con trỏ
		gotoXY(14, 37);
		setColor(10);//chữ xanh lá cây
		cout << "NHAP LAI: ENTER";
		q.nhapLai = 1;
		setColor(15);// màu trắng
		return;
	}
	else if (IsTuCoTonTaiTrongTuDien(td, tentu))// kiểm tra tên có tồn tại sẵn trong từ điển
	{
		veKhung(78, 12, 25, 2);
		gotoXY(82, 13);
		setColor(4);//chữ đỏ
		cout << "XXX: TU DA TON TAI";
		Cursor(false);// đóng con trỏ
		gotoXY(14, 37);
		setColor(10);//chữ xanh lá cây
		cout << "NHAP LAI: ENTER";
		q.nhapLai = 1;
		setColor(15);// màu trắng
		return;
	}
	q.Ok = q.nhapLai = 1;
}
// nhập loại từ, vẽ lại khung hiển thị sai  và lệnh tô đỏ
void xuLyLoaiTu(TUDIEN &td, string &loaitu)
{
	gotoXY(14, 37);
	for (int i = 1; i <= 15; i++)
	{
		cout << " ";
	}
	// xóa chữ NHAP LAI: ENTER
	setColor(10);//chữ xanh lá cây
	//màu trắng
	gotoXY(15, 37);
	cout << "CHON: ENTER";

	//veKhung(37, 12, 40, 2);
	veKhung(37, 15, 40, 2);
	//gotoXY((widthConSoLe / 2) - 20, 13);
	gotoXY((120 / 2) - 20, 16);
	cout << "LOAI TU (v/n/adj/adv): ";
	Cursor(true);// mở con trỏ
	getline(cin, loaitu); // nhập loại từ
	strlwr((char*)loaitu.c_str());// chuyển từ về chữ thường vĩnh viễn
	if (loaitu == "")// bỏ trống loại từ
	{
		//veKhung(78, 12, 25, 2);
		veKhung(78, 15, 25, 2);
		gotoXY(79, 16);
		setColor(4);//chữ đỏ
		cout << "XXX: KHONG DUOC BO TRONG";
		Cursor(false);// đóng con trỏ
		gotoXY(14, 37);
		setColor(10);//chữ xanh lá cây
		cout << "NHAP LAI: ENTER";
		q.nhapLai = 1;
		setColor(15);// màu trắng
		return;
	}
	else if (loaitu != "v" && loaitu != "n" && loaitu != "adj" && loaitu != "adv")// kiểm tra cú pháp từ
	{
		//veKhung(78, 12, 25, 2);
		veKhung(78, 15, 25, 2);
		gotoXY(82, 16);
		setColor(4);//chữ đỏ
		cout << "XXX: SAI CU PHAP";
		Cursor(false);// đóng con trỏ
		gotoXY(14, 37);
		setColor(10);//chữ xanh lá cây
		cout << "NHAP LAI: ENTER";
		q.nhapLai = 1;
		setColor(15);// màu trắng
		return;
	}
	q.Ok = q.nhapLai = 1;
}
void xuLyNghiaTu(string &nghiatu)
{
	gotoXY(14, 37);
	for (int i = 1; i <= 15; i++)
	{
		cout << " ";
	}
	// xóa chữ NHAP LAI: ENTER
	setColor(10);//chữ xanh lá cây
	//màu trắng
	gotoXY(15, 37);
	cout << "CHON: ENTER";

	veKhung(37, 18, 40, 2);
	gotoXY((120 / 2) - 20, 19);
	cout << "CAC NGHIA(,): ";
	Cursor(true);// mở con trỏ
	getline(cin, nghiatu); // nhập các nghĩa
	strlwr((char*)nghiatu.c_str());// chuyển từ về chữ thường vĩnh viễn

	if (nghiatu == "")// bỏ trống loại từ
	{
		//veKhung(78, 12, 25, 2);
		veKhung(78, 18, 25, 2);
		gotoXY(79, 19);
		setColor(4);//chữ đỏ
		cout << "XXX: KHONG DUOC BO TRONG";
		Cursor(false);// đóng con trỏ
		gotoXY(14, 37);
		setColor(10);//chữ xanh lá cây
		cout << "NHAP LAI: ENTER";
		q.nhapLai = 1;
		setColor(15);// màu trắng
		return;
	}
	q.Ok = q.nhapLai = 1;
}
void xuLyViDu(string &vidu, short number)
{
	// xóa chữ NHAP LAI: ENTER
	setColor(10);//chữ xanh lá cây
	//màu trắng
	gotoXY(8, 37);
	cout << "NHAP LAI/THEM VI DU: ENTER";

	switch (number)
	{
	case 1:// vi dụ 1
	{
			   veKhung(22, 21, 55, 2);
			   gotoXY((120 / 2) - 35, 22);
			   cout << "VI DU 1: ";
			   Cursor(true);// mở con trỏ
			   getline(cin, vidu); // nhập các nghĩa
			   strlwr((char*)vidu.c_str());// chuyển từ về chữ thường vĩnh viễn

			   if (vidu == "")// bỏ trống loại từ
			   {
				   veKhung(78, 21, 25, 2);
				   gotoXY(79, 22);
				   setColor(4);//chữ đỏ
				   cout << "XXX: KHONG DUOC BO TRONG";
				   Cursor(false);// đóng con trỏ
				   q.nhapLai = 1;
				   setColor(15);// màu trắng
				   return;
			   }
			   q.Ok = q.nhapLai = 1;
	}
		break;
	case 2:// vi dụ 2
	{
			   veKhung(22, 24, 55, 2);
			   gotoXY((120 / 2) - 35, 25);
			   cout << "VI DU 2: ";
			   Cursor(true);// mở con trỏ
			   getline(cin, vidu); // nhập ví dụ 2
			   strlwr((char*)vidu.c_str());// chuyển từ về chữ thường vĩnh viễn

			   if (vidu == "")// bỏ trống loại từ
			   {
				   veKhung(78, 24, 25, 2);
				   gotoXY(79, 25);
				   setColor(4);//chữ đỏ
				   cout << "XXX: KHONG DUOC BO TRONG";
				   Cursor(false);// đóng con trỏ
				   q.nhapLai = 1;
				   setColor(15);// màu trắng
				   return;
			   }
			   q.Ok = q.nhapLai = 1;
	}
		break;
	case 3:// vi dụ 3
	{
			   veKhung(22, 27, 55, 2);
			   gotoXY((120 / 2) - 35, 28);
			   cout << "VI DU 3: ";
			   Cursor(true);// mở con trỏ
			   getline(cin, vidu); // nhập ví dụ 2
			   strlwr((char*)vidu.c_str());// chuyển từ về chữ thường vĩnh viễn

			   if (vidu == "")// bỏ trống loại từ
			   {
				   veKhung(78, 27, 25, 2);
				   gotoXY(79, 28);
				   setColor(4);//chữ đỏ
				   cout << "XXX: KHONG DUOC BO TRONG";
				   Cursor(false);// đóng con trỏ
				   q.nhapLai = 1;
				   setColor(15);// màu trắng
				   return;
			   }
			   q.Ok = q.nhapLai = 1;
	}
		break;
	case 4:// vi dụ 4
	{
			   veKhung(22, 30, 55, 2);
			   gotoXY((120 / 2) - 35, 31);
			   cout << "VI DU 4: ";
			   Cursor(true);// mở con trỏ
			   getline(cin, vidu); // nhập ví dụ 2
			   strlwr((char*)vidu.c_str());// chuyển từ về chữ thường vĩnh viễn

			   if (vidu == "")// bỏ trống loại từ
			   {
				   veKhung(78, 30, 25, 2);
				   gotoXY(79, 31);
				   setColor(4);//chữ đỏ
				   cout << "XXX: KHONG DUOC BO TRONG";
				   Cursor(false);// đóng con trỏ
				   q.nhapLai = 1;
				   setColor(15);// màu trắng
				   return;
			   }
			   q.Ok = q.nhapLai = 1;
	}
		break;
	case 5:// vi dụ 5
	{
			   veKhung(22, 33, 55, 2);
			   gotoXY((120 / 2) - 35, 34);
			   cout << "VI DU 5 (CUOI CUNG): ";
			   Cursor(true);// mở con trỏ
			   getline(cin, vidu); // nhập ví dụ 2
			   strlwr((char*)vidu.c_str());// chuyển từ về chữ thường vĩnh viễn

			   if (vidu == "")// bỏ trống loại từ
			   {
				   veKhung(78, 33, 25, 2);
				   gotoXY(79, 34);
				   setColor(4);//chữ đỏ
				   cout << "XXX: KHONG DUOC BO TRONG";
				   Cursor(false);// đóng con trỏ
				   q.nhapLai = 1;
				   setColor(15);// màu trắng
				   return;
			   }
			   q.Ok = q.nhapLai = 1;
	}
		break;
	}
}
void xuLyViDuSai(short number)
{
	switch (number)
	{
	case 1:// vi dụ 1
	{
			   deleteRow(21, 3);
			   char d = 186;// dọc
			   setColor(6);
			   for (short i = 21; i <= 23; i++)
			   {
				   gotoXY(7, i);
				   cout << d;
				   gotoXY(107, i);
				   cout << d;
			   }
	}
		break;
	case 2:// vi dụ 2
	{
			   deleteRow(24, 3);
			   char d = 186;// dọc
			   setColor(6);
			   for (short i = 24; i <= 26; i++)
			   {
				   gotoXY(7, i);
				   cout << d;
				   gotoXY(107, i);
				   cout << d;
			   }
	}
		break;
	case 3:// vi dụ 3
	{
			   deleteRow(27, 3);
			   char d = 186;// dọc
			   setColor(6);
			   for (short i = 27; i <= 29; i++)
			   {
				   gotoXY(7, i);
				   cout << d;
				   gotoXY(107, i);
				   cout << d;
			   }
	}
		break;
	case 4:// vi dụ 4
	{
			   deleteRow(30, 3);
			   char d = 186;// dọc
			   setColor(6);
			   for (short i = 30; i <= 32; i++)
			   {
				   gotoXY(7, i);
				   cout << d;
				   gotoXY(107, i);
				   cout << d;
			   }
	}
		break;
	case 5:// vi dụ 5
	{
			   deleteRow(33, 3);
			   char d = 186;// dọc
			   setColor(6);
			   for (short i = 33; i <= 35; i++)
			   {
				   gotoXY(7, i);
				   cout << d;
				   gotoXY(107, i);
				   cout << d;
			   }
	}
		break;
	}
}
void xuLyKhungSave(short number)
{
	// vẽ khung báo SAVE
	switch (number)
	{
	case 1:
	{
			  veKhung(78, 21, 25, 2);
			  gotoXY(79, 22);
			  setColor(10);//chữ xanh lá cây
			  cout << "NHAN 'INSERT' DE SAVE TU";
			  break;
	}
	case 2:
	{
			  veKhung(78, 24, 25, 2);
			  gotoXY(79, 25);
			  setColor(10);//chữ xanh lá cây
			  cout << "NHAN 'INSERT' DE SAVE TU";
			  break;
	}
	case 3:
	{
			  veKhung(78, 27, 25, 2);
			  gotoXY(79, 28);
			  setColor(10);//chữ xanh lá cây
			  cout << "NHAN 'INSERT' DE SAVE TU";
			  break;
	}
	case 4:
	{
			  veKhung(78, 30, 25, 2);
			  gotoXY(79, 31);
			  setColor(10);//chữ xanh lá cây
			  cout << "NHAN 'INSERT' DE SAVE TU";
			  break;
	}
	}
}
void saoChepTu(TU &h)
{
	h.ten = q.them.ten;
	h.loaiTu = q.them.loaiTu;
	for (NODENGHIA *k = q.them.pHead; k != NULL; k = k->pNext)
	{
		themNodeNghia(h.pHead, khoiTaoNodeNghia(q.them.pHead->tenNghia));
	}
	for (int i = 1; i < q.them.lvd.n; i++)
	{
		h.lvd.viDu[i] = new string{ *q.them.lvd.viDu[i] };
		h.lvd.n++;
	}
}
void themTuMoiVaoTuDien(TUDIEN &td)
{
	if (q.VT == 1)
	{
		if (q.Ok == 0 && q.nhapLai == 0)// bắt đầu nhập
		{
			xuLyTenTu(td, q.them.ten);
			// nếu nhập từ sai
			if (q.Ok == 0)
			{
				return;
			}
		}
		if (q.Ok == 0 && q.nhapLai == 1)// xử lý nhập sai rồi nhập lại
		{
			// xóa đi cái hiển thị sai và nhập lại tên từ
			// xóa dòng và vẽ khung lại
			deleteRow(12, 3);
			char d = 186;// dọc
			setColor(6);
			for (short i = 12; i <= 14; i++)
			{
				gotoXY(7, i);
				cout << d;
				gotoXY(107, i);
				cout << d;
			}
			xuLyTenTu(td, q.them.ten);
			// nếu nhập từ sai
			if (q.Ok == 0)
			{
				return;
			}
		}
		if (q.Ok == 1)// nếu nhập từ đúng
		{
			q.Ok = q.nhapLai = 0;
			q.VT = 2;
		}
	}
	if (q.VT == 2)
	{
		if (q.Ok == 0 && q.nhapLai == 0)// bắt đầu nhập
		{
			xuLyLoaiTu(td, q.them.loaiTu);
			if (q.Ok == 0)
			{
				return;
			}
		}
		if (q.Ok == 0 && q.nhapLai == 1)// xử lý nhập sai rồi nhập lại
		{
			// xóa đi cái hiển thị sai và nhập lại tên từ
			// xóa dòng và vẽ khung lại
			deleteRow(15, 3);
			char d = 186;// dọc
			setColor(6);
			for (short i = 15; i <= 17; i++)
			{
				gotoXY(7, i);
				cout << d;
				gotoXY(107, i);
				cout << d;
			}
			xuLyLoaiTu(td, q.them.loaiTu);
			if (q.Ok == 0)
			{
				return;
			}
		}
		if (q.Ok == 1)// nếu nhập từ đúng
		{
			if (q.them.loaiTu == "v") q.them.loaiTu = "Dong tu";
			else if (q.them.loaiTu == "n") q.them.loaiTu = "Danh tu";
			else if (q.them.loaiTu == "adj") q.them.loaiTu = "Tinh tu";
			else if (q.them.loaiTu == "adv") q.them.loaiTu = "Trang tu";
			q.Ok = q.nhapLai = 0;
			q.VT = 3;
		}
	}
	if (q.VT == 3)
	{
		if (q.Ok == 0 && q.nhapLai == 0)// bắt đầu nhập
		{
			xuLyNghiaTu(q.tu);
			if (q.Ok == 0)
			{
				return;
			}
		}
		if (q.Ok == 0 && q.nhapLai == 1)// xử lý nhập sai rồi nhập lại
		{
			// xóa đi cái hiển thị sai và nhập lại tên từ
			// xóa dòng và vẽ khung lại
			deleteRow(18, 3);
			char d = 186;// dọc
			setColor(6);
			for (short i = 18; i <= 20; i++)
			{
				gotoXY(7, i);
				cout << d;
				gotoXY(107, i);
				cout << d;
			}
			xuLyNghiaTu(q.tu);
			if (q.Ok == 0)
			{
				return;
			}
		}
		if (q.Ok == 1)// nếu nhập từ đúng
		{
			string nghia = "";
			for (int i = 0; i <= q.tu.length(); i++)
			{
				if (q.tu[i] != ',' && q.tu[i] != '\0')
				{
					nghia.push_back(q.tu[i]);
				}
				else
				{
					themNodeNghia(q.them.pHead, khoiTaoNodeNghia(nghia));
					nghia = "";
				}
			}
			q.Ok = q.nhapLai = 0;
			q.VT = 4;
		}
	}
	if (q.VT == 4)
	{
		if (q.Ok == 0 && q.nhapLai == 0)// bắt đầu nhập
		{
			xuLyViDu(q.tu, q.them.lvd.n);// vd 1
			if (q.Ok == 0)
			{
				return;
			}
		}
		if (q.Ok == 0 && q.nhapLai == 1)// xử lý nhập sai rồi nhập lại
		{
			xuLyViDuSai(q.them.lvd.n);
			xuLyViDu(q.tu, q.them.lvd.n);
			if (q.Ok == 0)
			{
				return;
			}
		}
		if (q.Ok == 1)// nếu nhập từ đúng
		{
			q.them.lvd.viDu[q.them.lvd.n] = new string{ q.tu };
			xuLyKhungSave(q.them.lvd.n);
			q.them.lvd.n++;
			Cursor(false);
			q.Ok = q.nhapLai = 0;
			if (q.them.lvd.n == maxVidu)
			{
				TU h;
				saoChepTu(h);
				themTuVaoTuDien(td, h);
				giaiPhong1Tu(q.them);
				q.them.pHead = NULL;
				q.them.lvd.n = 1;
				STrang = 1;
				SViTriChon = 1;
				STongMuc = 3;
				q.Ok = 0;
				q.nhapLai = 0;
				q.VT = 1;
				q.isThemTu = 0;
				setColorBGTextXY(42, 15, 0, 10, "   DA LUU TU MOI VAO TU DIEN   ");
				Sleep(2000);
				setColorBGTextXY(0, 0, 0, 0, " ");
				system("cls");
				manHinh1();
			}
		}
	}
}


// ================ PHẦN TÌM KIẾM SEARCH ====================

// Các biến trong xử lý dịch chuyển thanh sáng
struct xuLyMenuThanhSangVaChonTu
{
	short sttbd = 1;
	short sttt = 1;
	string search = "";// tu search
	short SVitriCu = 1;
	short sttbdCu = 1;
	short vtnhay = 24;
	short svtc = 1;
	short stongmuc = 0;
	NODETU *k = NULL;
} w;

// Phần màn hình 2 (tìm kiếm)

// vẽ chữ Tìm kiếm
void veTieuDeMH2(short mau)
{
	setColor(mau);// màu 
	ifstream filein;
	filein.open("vehinh2.txt", ios_base::in);
	short i = 1;
	string a;
	while (!filein.eof())
	{
		getline(filein, a);
		gotoXY(25, i++);
		cout << a << endl;
	}
	filein.close();
	setColor(15);// màu trắng
}
//Vẽ khung search
void veKhungMH2search()
{
	veKhung(15, 10, 25, 23);
	// In ra ngang giữa (ranh giới để search và tìm)
	gotoXY(16, 12);
	setColor(6);// màu cam
	cout << string(24, (char)205);
	// Ghi chữ search
	setColor(15);// màu trắng
	gotoXY(16, 11);
	cout << "Search: ";

}
// hàm kiểm tra chuỗi con có tồn tại từ vt đầu trỏ đi
bool ktChuoiCon(string con, string cha)
{
	if (con.length() > cha.length()) return 0;
	else
	{
		for (short i = 0; i < con.length(); i++)
		{
			if (con[i] != cha[i]) return 0;
		}
		return 1;
	}
}
void menuSearch2a(TUDIEN &td);
// xử lý hiện tự danh sách từ khi search
void menuSearch2b(TUDIEN &td, string search, short svtc)
{
	short dem = 1;// số lượng từ
	short vtdt = 13;// vt từ đầu tiên
	short vtDay = (short)search[0] - 96;// vt dãy

	SViTriChon = 1;
	if (search != "")
	{
		for (NODETU *k = td.dSDay[vtDay].pHead; k != NULL; k = k->pNext)// chạy hết cái dãy mà mình xâm nhập
		{
			if (ktChuoiCon(search, k->data.ten))
			{
				if (dem == w.svtc)
				{
					setColorBGTextXY(16, vtdt, 15, 3, "                        ");
					setColorBGTextXY(16, vtdt, 15, 3, (LPSTR)k->data.ten.c_str());
					setColorBGTextXY(0, 0, 0, 0, "");
				}
				else
				{
					setColorBGTextXY(16, vtdt, 15, 0, "                        ");
					gotoXY(16, vtdt);
					setColor(15);
					cout << k->data.ten;
				}
				dem++;
				vtdt++;
			}
		}
	}
	else
	{
		menuSearch2a(td);
	}
}
// Tổng số từ chọn khi search
short tongChon(TUDIEN &td, string search)
{
	short dem = 0;
	short vtDay = (short)search[0] - 96;// vt dãy

	if (search != "")
	{
		for (NODETU *k = td.dSDay[vtDay].pHead; k != NULL; k = k->pNext)
		{
			if (ktChuoiCon(search, k->data.ten))
			{
				dem++;
			}
		}
	}
	return dem;
}
// vẽ 20 từ tương ứng với thanh sáng SVitrichon
void menuSearch2a(TUDIEN &td)
{
	short dem = 0;// số lượng từ
	short vtDay = 1;// vt dãy
	short vtdt = 13;// vt từ đầu tiên

	NODETU* k = td.dSDay[vtDay].pHead;
	for (short i = 1; i < w.sttbd; i++)// tìm từ bắt đầu
	{
		k = k->pNext;
		while (k == NULL)
		{
			k = td.dSDay[++vtDay].pHead;
		}
	}

	while (1)
	{
		for (; k != NULL; k = k->pNext)
		{
			dem++;
			if (dem > 20)
			{
				// nháy để search
				gotoXY(24, 11);
				Cursor(true);
				return;
			}
			if (dem == SViTriChon)
			{
				setColorBGTextXY(16, vtdt, 15, 3, "                        ");
				setColorBGTextXY(16, vtdt, 15, 3, (LPSTR)k->data.ten.c_str());
				setColorBGTextXY(0, 0, 0, 0, "");
			}
			else
			{
				setColorBGTextXY(16, vtdt, 15, 0, "                        ");
				gotoXY(16, vtdt);
				cout << k->data.ten;
			}
			vtdt++;
		}
		if (dem <= 20)
		{
			k = td.dSDay[++vtDay].pHead;
		}
	}
}
void manHinh2(TUDIEN &td)
{
	veKhung12();// vẽ khung lớn
	veNganCacPhimMH12();// vẽ ngăn lệnh
	veLenhMH12(3, "TRO VE: ESC");// Ghi các lệnh// màu xanh dương
	veTieuDeMH2(3);// ghi tiêu đề// màu xanh dương
	veKhungMH2search();//Vẽ khung search
	menuSearch2a(td);// vẽ menu để search ban đầu (đầu tiên)
}

// ================ PHẦN MENU 3 ====================

void menu3(short SInDex)// menu chọn xử lý từ
{
	// cập nhật lại vị trí đang chọn
	SViTriChon = SInDex;

	// vẽ khung
	veKhung(65, 10, 18, 5);

	// vẽ menu
	LPSTR STRTextMenuChinh = "  XEM CHI TIET   ";
	setColorBGTextXY((120 / 2) - (strlen(STRTextMenuChinh) / 2) + 14, 11, 15, (SInDex == 1) ? 3 : 0, STRTextMenuChinh);

	STRTextMenuChinh = "  CHINH SUA TU   ";
	setColorBGTextXY((120 / 2) - (strlen(STRTextMenuChinh) / 2) + 14, 12, 15, (SInDex == 2) ? 3 : 0, STRTextMenuChinh);

	STRTextMenuChinh = "     XOA TU      ";
	setColorBGTextXY((120 / 2) - (strlen(STRTextMenuChinh) / 2) + 14, 13, 15, (SInDex == 3) ? 3 : 0, STRTextMenuChinh);

	STRTextMenuChinh = "     TRO VE      ";
	setColorBGTextXY((120 / 2) - (strlen(STRTextMenuChinh) / 2) + 14, 14, 15, (SInDex == 4) ? 3 : 0, STRTextMenuChinh);

	setColorBGTextXY(0, 0, 15, 0, "");
}
void xoaTu(TUDIEN &td)
{
	string ten = w.k->data.ten;
	short vt = short(ten[0]) - 96;
	if (td.dSDay[vt].pHead->data.ten == ten)// từ đầu dãy
	{
		xoaDauDay(td.dSDay[vt]);
		return;
	}
	for (NODETU *h1 = td.dSDay[vt].pHead; h1 != NULL; h1 = h1->pNext)// từ cuối dãy
	{
		if (h1->pNext == NULL && h1->data.ten == ten)
		{
			xoaCuoiDay(td.dSDay[vt]);
			return;
		}
	}
	
	NODETU* tam = w.k;
	w.k->pRev->pNext = w.k->pNext;
	w.k->pNext->pRev = w.k->pRev;
	delete tam;
}
void xoaTu2(TUDIEN &td, string search, short svtc)
{
	short vt = short(search[0]) - 96;
	if (td.dSDay[vt].pHead == w.k)// từ đầu dãy
	{
		xoaDauDay(td.dSDay[vt]);
		return;
	}

	NODETU *h1;
	for (h1 = td.dSDay[vt].pHead; h1->pNext != NULL; h1 = h1->pNext);// từ cuối dãy
	if (h1 == w.k)
	{
		xoaCuoiDay(td.dSDay[vt]);
		return;
	}

	NODETU* tam = w.k;
	w.k->pRev->pNext = w.k->pNext;
	w.k->pNext->pRev = w.k->pRev;
	delete tam;
}
void menu4(short SInDex)//menu chỉnh từ
{
	// cập nhật lại vị trí đang chọn
	SViTriChon = SInDex;

	// vẽ menu 4
	gotoXY(50, 22);
	cout << "- TEN TU:  " << w.k->data.ten;

	gotoXY(50, 23);
	cout << "- LOAI TU: ";
	string slt = w.k->data.loaiTu + "     ";
	LPSTR STRTextMenuChinh = (char*)(slt).c_str();
	setColorBGTextXY(61, 23, 15, (SInDex == 1) ? 3 : 0, STRTextMenuChinh);
	setColorBGTextXY(0, 0, 15, 0, "");

	gotoXY(50, 24);
	cout << "- NGHIA: ";
	string sn = "";
	for (NODENGHIA *h = w.k->data.pHead; h != NULL; h = h->pNext)
	{
		sn = sn + h->tenNghia;
		if (h->pNext != NULL)
		{
			sn += ", ";
		}
	}
	sn += "     ";
	STRTextMenuChinh = (char*)sn.c_str();
	setColorBGTextXY(61, 24, 15, (SInDex == 2) ? 3 : 0, STRTextMenuChinh);
	setColorBGTextXY(0, 0, 15, 0, "");

	gotoXY(50, 25);
	cout << "- VI DU: ";
	short vt = 26;
	cout << endl;
	for (short i = 1; i < w.k->data.lvd.n; i++)
	{
		gotoXY(59, vt++);
		cout << "+ " << *w.k->data.lvd.viDu[i];
	}
	string svd = *(w.k->data.lvd.viDu[1]) + "          ";
	STRTextMenuChinh = (char*)(svd).c_str();
	setColorBGTextXY(61, 26, 15, (SInDex == 3) ? 3 : 0, STRTextMenuChinh);
	setColorBGTextXY(0, 0, 15, 0, "");

	if (SInDex == 1)
	{
		w.vtnhay = 61 + slt.length() - 5;
		gotoXY(w.vtnhay, 23);
	}
	else if (SInDex == 2)
	{
		w.vtnhay = 61 + sn.length() - 5;
		gotoXY(w.vtnhay, 24);
	}
	else if (SInDex == 3)
	{
		w.vtnhay = 61 + svd.length() - 10;
		gotoXY(w.vtnhay, 26);
	}
	Cursor(true);
}
void setUpLaiMenu2(TUDIEN &td)
{
	system("cls");
	STrang = 2;
	STongMuc = 20;
	SViTriChon = w.SVitriCu;
	w.sttbd = w.sttbdCu;
	manHinh2(td);

	w.search = "";
	w.svtc = 1;
	w.vtnhay = 24;
	if (isSearch == true) w.sttt = 1;
	isSearch = false;
}
// ================ PHẦN XỬ LY PHIM ====================

void xuLyPhim(KEY_EVENT_RECORD key, TUDIEN &td)
{
	if (key.bKeyDown)// nếu có nhấn phím
	{
		switch (key.wVirtualKeyCode)
		{
		case VK_UP:// Phím lên
			switch (STrang)
			{
			case 1: // Menu Chinh
			{
						if (STongMuc == 3 && q.isThemTu != 1)
						{
							if (SViTriChon == 1)
							{
								SViTriChon = STongMuc;
							}
							else
							{
								SViTriChon -= 1;
							}
							veMenuMH1(SViTriChon);
						}
			}
				break;
			case 2: // Menu Tìm kiếm
			{
						if (isSearch == false)
						{
							if (w.sttt != 1)// xử lý số thứ tự từ
							{
								w.sttt -= 1;// giảm số từ lên 1
							}
							if (SViTriChon > 1)// thanh sáng khác vị trí đầu
							{
								SViTriChon -= 1;
								menuSearch2a(td);
							}
							else if (w.sttbd != 1)// thanh sáng ở vị trí đầu
							{
								w.sttbd -= 1;
								menuSearch2a(td);
							}
							gotoXY(27, 10);
							cout << "  ";
							setColor(3);
							gotoXY(27, 10);
							cout << w.sttt;
							// nháy để search
							gotoXY(24, 11);
							Cursor(true);
						}
						else
						{
							if (w.svtc != 1)
							{
								w.svtc -= 1;
							}
							menuSearch2b(td, w.search, w.svtc);
							// nháy để search
							gotoXY(w.vtnhay, 11);
							Cursor(true);
						}
			}
				break;
			case 3: // Menu 3 xử lý từ
			{
						if (SViTriChon == 1)
						{
							SViTriChon = STongMuc;
						}
						else
						{
							SViTriChon -= 1;
						}
						menu3(SViTriChon);
			}
				break;
			case 4:// Menu Chỉnh từ
			{
					   if (SViTriChon == 1)
					   {
						   SViTriChon = STongMuc;
					   }
					   else
					   {
						   SViTriChon -= 1;
					   }
					   menu4(SViTriChon);
			}
				break;
			}
			break;

		case VK_DOWN:// Phím xuống
			switch (STrang)
			{
			case 1: // Menu Chinh
			{
						if (STongMuc == 3 && q.isThemTu != 1)
						{
							if (SViTriChon == 3)
							{
								SViTriChon = 1;
							}
							else
							{
								SViTriChon += 1;
							}
							veMenuMH1(SViTriChon);
						}
			}
				break;
			case 2: // Menu Tìm kiếm
			{
						if (isSearch == false)
						{
							if (w.sttt < soLuongTu(td))// xử lý số thứ tự từ
							{
								w.sttt += 1;// tăng số từ lên 1
							}
							if (SViTriChon < 20)//nếu thanh sáng khác vị trí cuối
							{
								SViTriChon += 1;
								menuSearch2a(td);
							}
							else//nếu thanh sáng ở vị trí cuối
							{
								if (soLuongTu(td) - 19 != w.sttbd)
								{
									w.sttbd += 1;
									menuSearch2a(td);
								}
							}
							gotoXY(27, 10);
							cout << "  ";
							setColor(3);
							gotoXY(27, 10);
							cout << w.sttt;
							// nháy để search
							gotoXY(24, 11);
							Cursor(true);
						}
						else
						{
							if (w.svtc != w.stongmuc)
							{
								w.svtc += 1;
							}
							menuSearch2b(td, w.search, w.svtc);
							// nháy để search
							gotoXY(w.vtnhay, 11);
							Cursor(true);
						}
			}
				break;
			case 3: // menu 3 xử lý từ được chọn
			{
						if (SViTriChon == STongMuc)
						{
							SViTriChon = 1;
						}
						else
						{
							SViTriChon += 1;
						}
						menu3(SViTriChon);
			}
				break;
			case 4:// Menu Chỉnh từ
			{
					   if (SViTriChon == STongMuc)
					   {
						   SViTriChon = 1;
					   }
					   else
					   {
						   SViTriChon += 1;
					   }
					   menu4(SViTriChon);
			}
				break;
			}
			break;

		case VK_RETURN:// Phím enter
			switch (STrang)
			{
			case 1: // Menu Chinh
			{
						if (STongMuc == 3)
						{
							if (SViTriChon == 1)// chọn tìm kiếm
							{
								STrang = 2;
								SViTriChon = 1;
								STongMuc = 20;
								system("cls");
								manHinh2(td);

								w.search = "";
								w.svtc = 1;
								w.vtnhay = 24;
								w.sttt = 1;
								isSearch = false;
							}
							else if (SViTriChon == 2)// chọn thêm từ mới vào từ điển
							{
								q.isThemTu = 1;
								themTuMoiVaoTuDien(td);
							}
							else
							{
								giaiPhongTuDien(td);
								exit(0);
							}
						}
			}
				break;
			case 2: // Menu Search
			{
						// GIỮ LẠI CÁI CŨ
						w.SVitriCu = SViTriChon;
						w.sttbdCu = w.sttbd;

						Cursor(false);
						STrang = 3;
						STongMuc = 4;
						SViTriChon = 1;
						menu3(1);

						if (isSearch == false)
						{
							short vtDay = 1;// vt dãy
							w.k = td.dSDay[vtDay].pHead;
							for (short i = 1; i < w.sttt; i++)// tìm từ bắt đầu
							{
								w.k = w.k->pNext;
								while (w.k == NULL)
								{
									w.k = td.dSDay[++vtDay].pHead;
								}
							}
						}
						else
						{
							short vt = short(w.search[0]) - 96;

							for (w.k = td.dSDay[vt].pHead; w.k != NULL; w.k = w.k->pNext)
							{
								if (ktChuoiCon(w.search, td.dSDay[vt].pHead->data.ten))
								{
									break;
								}
							}

							for (int i = 1; i<w.svtc; i++)
							{
								w.k = w.k->pNext;
							}
						}
			}
				break;
			case 3: // Chọn trong menu 3
			{

						if (SViTriChon == 1)// Hiển thị thong tin từ
						{
							xuat1Tu(w.k->data);
							Cursor(false);
						}
						else if (SViTriChon == 2)// CHINH SUA
						{
							STrang = 4;
							STongMuc = 3;
							SViTriChon = 1;
							menu4(SViTriChon);
						}
						else if (SViTriChon == 3)// XÓA TỪ
						{
							if (isSearch == false)
							{
								xoaTu(td);
							}
							else
							{
								xoaTu2(td, w.search, w.svtc);
							}
							setColorBGTextXY(42, 15, 0, 10, "   DANG XOA TU RA KHOI TU DIEN   ");
							Sleep(2000);
							setColorBGTextXY(0, 0, 0, 0, " ");

							system("cls");
							STrang = 2;
							SViTriChon = 1;
							STongMuc = 20;
							w.sttbd = 1;
							w.sttt = 1;
							manHinh2(td);
							w.search = "";
							w.svtc = 1;
							w.vtnhay = 24;
							isSearch = false;
						}
						else if (SViTriChon == 4)// TRỞ VỀ
						{
							setUpLaiMenu2(td);
						}
			}
				break;
			}
			break;

		case VK_INSERT:
		{
						  if (STrang == 1 && q.VT == 4)
						  {
							  STrang = 1;
							  SViTriChon = 1;
							  STongMuc = 3;
							  q.Ok = 0;
							  q.nhapLai = 0;
							  q.VT = 1;
							  q.isThemTu = 0;
							  TU h;
							  saoChepTu(h);
							  themTuVaoTuDien(td, h);
							  giaiPhong1Tu(q.them);
							  q.them.pHead = NULL;
							  q.them.lvd.n = 1;
							  setColorBGTextXY(42, 15, 0, 10, "   DANG LUU TU MOI VAO TU DIEN   ");
							  Sleep(2000);
							  setColorBGTextXY(0, 0, 0, 0, " ");
							  system("cls");
							  manHinh1();
						  }
		}
			break;

		case VK_ESCAPE:// phím Esc
			switch (STrang)
			{
			case 1: // Menu Chinh
			{
						if (q.isThemTu == 1)// Đang ở chế độ thêm từ
						{
							STrang = 1;
							SViTriChon = 1;
							STongMuc = 3;
							q.Ok = 0;
							q.nhapLai = 0;
							q.VT = 1;
							q.isThemTu = 0;
							system("cls");
							manHinh1();
							return;
						}
						if (STongMuc == 3)// chế độ trang chủ
						{
							giaiPhongTuDien(td);
							exit(0);
						}
			}
				break;
			case 2:// menu tìm kiếm
			{
					   system("cls");
					   manHinh1();
					   STrang = 1;
					   SViTriChon = 1;
					   STongMuc = 3;
					   q.Ok = 0;
					   q.nhapLai = 0;
					   q.VT = 1;
					   q.isThemTu = 0;
			}
				break;
			case 3:// menu 3 xử lý từ
			case 4:// menu 4 sửa từ
			{
				setUpLaiMenu2(td);
			}
				break;
			}
			break;

		default:// nhập chữ
		{
					if (STrang == 2)
					{
						isSearch = true;

						char x = (char)key.wVirtualKeyCode + 32;
						if ((int)x == 40)// backspace
						{
							if (w.vtnhay != 24)
							{
								gotoXY(--w.vtnhay, 11);//canh
								cout << " ";
								w.search.erase(w.search.length() - 1);
								if (w.vtnhay == 24)
								{
									w.svtc = 1;
									w.search = "";
									w.vtnhay = 24;
									isSearch = false;
									w.sttt = 1;
								}
							}
							else
							{
								w.svtc = 1;
							}
						}
						else// kí tự
						{
							setColor(2);
							cout << x;
							w.search.push_back(x);
							w.vtnhay++;
						}

						// xóa hết 20 dòng
						for (short VTDT = 13; VTDT <= 32; VTDT++)
						{
							setColorBGTextXY(16, VTDT, 15, 0, "                        ");
						}

						w.svtc = 1;
						w.stongmuc = tongChon(td, w.search);

						// HIỂN THỊ ALL NHỮNG TỪ BẮT ĐẦU TỪ W.SEARCH
						menuSearch2b(td, w.search, w.svtc);

						// nháy để search
						gotoXY(w.vtnhay, 11);
						Cursor(true);

						setColor(15);
					}
			}
		}
	}
}

void xuLySuKien(TUDIEN &td)
{
	while (1)
	{
		// typedef unsigned long DWORD;
		DWORD DWNumberOfEvents = 0; // Lưu lại sự kiện hiện tại
		DWORD DWNumberOfEventsRead = 0;// Lưu lại số lượng sự kiện đã được lọc

		// typedef PVOID HANDLE;
		// typedef void *PVOID;
		HANDLE HConsoleInput = GetStdHandle(STD_INPUT_HANDLE);// Thiết bị đầu vào
		// typedef DWORD *LPDWORD;
		GetNumberOfConsoleInputEvents(HConsoleInput, &DWNumberOfEvents);// Đặt sự kiện đầu vào của giao diện cho biến DWNumberOfEvents

		if (DWNumberOfEvents)// Nếu như có sự kiện
		{
			INPUT_RECORD *IREventbuffer = new INPUT_RECORD[DWNumberOfEvents];
			ReadConsoleInput(HConsoleInput, IREventbuffer, DWNumberOfEvents, &DWNumberOfEventsRead);// Thì đặt các sự kiện được lưu vào con trỏ IREventbuffer

			// Chạy vòng lặp để đọc sự kiện
			for (DWORD i = 0; i < DWNumberOfEvents; ++i)
			{
				if (IREventbuffer[i].EventType == KEY_EVENT)// Nếu là sử lý sự kiện phím
				{
					xuLyPhim(IREventbuffer[i].Event.KeyEvent, td);
				}
				//else if (IREventbuffer[i].EventType == MOUSE_EVENT)// Nếu là sử lý sự kiện chuột
				//{

				//}
			}
		}
	}
}

int main()
{
	resizeConsole(widthConSoLe, heightConSoLe);// Thay đổi kích thước màn hình console
	SetConsoleTitle(TEXT("TU DIEN - HUYNH NGOC CANH"));// Thay đổi tiêu đề
	manHinh1();
	TUDIEN td;
	docFileVaoTuDien(td);
	xuLySuKien(td);
	system("pause");
	return 0;
}