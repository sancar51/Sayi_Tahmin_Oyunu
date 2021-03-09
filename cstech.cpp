// cstech.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*
 Bu uygulama bilgisayar ve kullanici arasýnda oynanan bir oyundur. 
Her iki tafta bir sayi tutar. Ýki tarafta birbirlerinin tuttugu sayilari tahmin etmeye calisir. 
Her el sirasiyla bir kullanici tahminde bulunur. 
Taraflar birbirlerinin tahminlerine bazý ipuclari verir.
Eger karsi tarafin tahminde bulundugu sayýda bulunan rakamlarýn:
*basamak degeri ve sayi degeri dogruysa "+1",
*sadece sayi degeri dogruysa "-1",
*basamak ve sayi degerinin ikiside yanlis ise "0" olarak karsi tarafa basamak basamak ip ucu verir.
*/

//Yogun mesaili calistigim icin arayüz yazma fýrsatim olmadi.



#include <iostream>
#include <cstdint>
#include <stdlib.h>
#include <time.h>
#include <math.h>






using namespace std;

struct
{
	uint64_t data;
	unsigned int  DigitCount;
}Common;
struct
{
	uint64_t Holding_Number;
	uint64_t Holding_Array[10];
	uint64_t Estimating_Number;
	uint64_t Estimating_Array[10];
	int dogruluk_tablosu[10];
}values;

struct
{
	uint64_t Estimating_Number;
	uint64_t Estimating_Array[10];
	int dogruluk_tablosu[10];
}user;
enum
{
	initialize,
	start,
	human,
	computer,
	gecersiz,
}Game_state;

bool Rakam[10];
bool tablo[10][10];
uint8_t eksi1[10];
uint8_t eksisayisi;


//Random sayi uretmek ýcýn kullandigim fonksiyon. degiskenlerin 64 bit iþaretsiz olarak belirlenmesi 10 basamakli sayýlara kadar çalýsmasidir.
//rand komutu maximum 32125 e kadar deger dondurdugu icin basamak basamak random sayý uretmek daha mantýkli oldu.
uint64_t Generate_Number(unsigned int digit_count, uint64_t* Array)
{
	uint64_t number = 0;
	bool difference = false;
	uint64_t max = 0;
	uint64_t min = 0;
	max = pow(10, digit_count);
	min = pow(10, digit_count-1);
	while (!difference)
	{
		difference = true;
		while (number < min)
		{
			uint64_t OnHaneliSayi = 0;
			uint8_t gecici = 0;
			for (size_t i = 0; i < 10; i++)
			{
				gecici = rand() % 10;
				OnHaneliSayi = OnHaneliSayi + gecici * pow(10, i);
			}
			number  = OnHaneliSayi % max;
		}
		uint64_t temp = number;
		int counter = 0;
		while (temp)
		{
			Array[counter] = temp % 10;
			temp = temp / 10;
			for (int i = 0;i < counter;i++)
			{
				if (Array[i] == Array[counter])
				{
					number = 0;
					temp = 0;
					difference = false;
				}

			}
			counter++;
		}
	}
	return number;
}


//kullanicinin girdigi sayinin rakamlari birbirinden farkli mi ve oyunun oynandigi basamak sayisina esitmi sorgulamasi yapiliyor.
bool Kurallara_Uygunmu (uint64_t data, uint64_t basamakSayisi)
{
	values.Estimating_Number = data;
	bool CorrectionOut = true;
	int counter = 0;
	while (data)
	{
		values.Estimating_Array[counter] = data % 10;
		data = data / 10;
		for (int i = 0;i < counter;i++)
		{
			if (values.Estimating_Array[i] == values.Estimating_Array[counter])
			{				
				CorrectionOut = false;
			}

		}		
		counter++;
	}
	if(counter != basamakSayisi) CorrectionOut = false;
	return CorrectionOut;
}


//kullanicinin yaptigi tahmine gore sonuclarin belirlendigi fonksiyon.
void Tahmin_Kontrolu(void)
{
	int counter = 0;
	while (counter < Common.DigitCount)
	{
		if (values.Holding_Array[counter] == values.Estimating_Array[counter])
		{
			values.dogruluk_tablosu[counter] = 1;
		}
		else
		{
			for (int i = 0;i < Common.DigitCount;)
			{
				if (values.Estimating_Array[counter] == values.Holding_Array[i])
				{
					values.dogruluk_tablosu[counter] = -1;
				}
				i++;
			}
		}
		counter++;

	}
}

//kullanýcýnýn tahmin sonuclarini ekrana yazdirir.
bool SonucGoster(void)
{
	bool IsSuccess = true;
	for (int i = 0;i < Common.DigitCount;i++)
	{
		if (values.dogruluk_tablosu[i]!=1)
		{
			IsSuccess = false;
		}
	}
	if (IsSuccess)
	{
		cout << endl << "************************" << endl<<endl;
		cout << "tebrikler sayiyi basarili bir sekilde tahmin ettiniz." << endl<<endl;
		cout << "oyun sonlandi" << endl<<endl;
	}
	else
	{
		cout << "sonuc raporu" << endl;
		for (int i = Common.DigitCount-1;i >=0;i--)
		{
			cout<<"  " << values.Estimating_Array[i];
		}
		cout << endl;
		for (int i = Common.DigitCount - 1;i >= 0;i--)
		{
			if (values.dogruluk_tablosu[i] < 0)	cout<<" " << values.dogruluk_tablosu[i];
			else cout << "  " << values.dogruluk_tablosu[i];
		}
		std::cout<< endl;
		std::cout<< "Bir sonraki tahmininizde yukaridaki ipuclarini goz onunde bulundurunuz.Tahmin sirasi bilgisayarda :)" << endl;
	}
	return IsSuccess;
}




//bilgisayarain yaptigi sayi tahminlerinin sonuclarina gore kullanicidan ipucu alan fonksiyon.
bool IpucuAl(void)
{
	bool IsSuccess = true;
	for (size_t i = 0; i < Common.DigitCount;)
	{
		int temp = 0xFF;
		cout << "simdi belirtilen rakam icin ipucu girin." << endl;
		cout << user.Estimating_Array[i] << " => " << endl;
		cin >> temp;
		if ((temp == 1) || (temp == -1) || (temp == 0))
		{
			user.dogruluk_tablosu[i] = temp;
			i++;
		}
		else
		{
			cout << " gecersiz deger girdiniz. lutfen yukaridaki belirtilen durumlara gore ipucu bilgisi giriniz."<<endl;
		}
	}
	for (size_t i = 0; i < Common.DigitCount; i++)
	{
		if (user.dogruluk_tablosu[i] != 1) IsSuccess = false;
	}
	return IsSuccess;


}

//bu fonksiyon kullanicinin verdigi ipuclarina gore yeni sayilar uretir. 
//bir tablo kullanir. oyun maximum 10 basamakli oldugu icin degiskenler bu sekilde tanimlandi.
//tabloda 10x10 boyutundadir. Verilen ipuclarina ve bir onceki elde yapilan tahminlerin sonucuna gore hangi basamaga hangi rakamlarýn gelebilecegini belirler. Yeni sayi uretimini bu tablodan yapar. 
//Bu noktadan sonra bilgisayarin tahminleri randomluktan çýkarýlmýþtýr.
void SayiTahminEt(void)
{
	user.Estimating_Number = 0;
	for (size_t i = 0; i < 10; i++)
	{
		Rakam[i] = 0;
		eksi1[i] = 0;
		eksisayisi = 0;
	}
	for (size_t i = 0; i < Common.DigitCount; i++)
	{
		if(user.dogruluk_tablosu[i]==1)
		{
			for (size_t j = 0; j < Common.DigitCount; j++)
			{
				tablo[j][user.Estimating_Array[i]] = 0;
			}
			for (size_t k = 0; k < 10; k++)
			{
				tablo[i][k] = 0;
			}
			tablo[i][user.Estimating_Array[i]] = 1;		
		}
		else if (user.dogruluk_tablosu[i] == 0)
		{
			for (size_t j = 0; j < Common.DigitCount; j++)
			{
				tablo[j][user.Estimating_Array[i]] = 0;
			}
		}
		else if (user.dogruluk_tablosu[i] == -1)
		{
			tablo[i][user.Estimating_Array[i]] = 0;
			eksi1[eksisayisi] = user.Estimating_Array[i];
			eksisayisi++;
		}
	}

	for (size_t i = 0; i < Common.DigitCount; i++)
	{
		bool varmi = 0;
		if (eksisayisi > 0)
		{
			for (size_t k = 0; k < eksisayisi; k++)
			{
				if ((tablo[i][eksi1[k]] == 1) && (Rakam[eksi1[k]] == 0))
				{
					varmi=true;
					user.Estimating_Array[i]= eksi1[k];
					Rakam[eksi1[k]]=true;
					break;
				}
			}
		}
		if(varmi) continue;
		for (size_t j = 0; j < 10; j++)
		{
			if ((tablo[i][j] == 1) && (Rakam[j] == 0))
			{
				user.Estimating_Array[i] = j;
				Rakam[j] = 1;
				break;
			}

		}
	}


	for (size_t i = 0; i < Common.DigitCount; i++)
	{
		user.dogruluk_tablosu[i] = 0;
		user.Estimating_Number = user.Estimating_Number + user.Estimating_Array[i]*pow(10, i);
	}
	if (!Kurallara_Uygunmu(user.Estimating_Number, Common.DigitCount))
		user.Estimating_Number=Generate_Number(Common.DigitCount,&user.Estimating_Array[0]);
}

//oyunun gorevlerinin yurutuldugu fonksiyondur. Durum makinesi seklinde calisir. diger alt fonksiyonlar  buradan cagrilir.
void Game(void)
{
	int a = 0;
	bool verfy = 1;
	switch (Game_state)
	{
	case initialize:

		cout << "oyuna baslamak icin 1'e basin." << endl;
		cin >> Common.data;
		if (Common.data != 1)
		{
			cout << "gecersiz karekter girildi." << endl;
			break;
		}
		cout << "oyunu kac basamakli oynamak istiyorsunuz? En az dort en fazla 10 basamak olmak uzere basamak degerini belirtiniz." << endl;
		cin >> Common.data;
		if ((Common.data>0) &&(Common.data <= 10))
		{
			Common.DigitCount = Common.data;
			cout << "Oyun " << Common.DigitCount << " basamakli olarak devam edecektir." << endl;
			cout << "Lutfen aklinizdan Rakamlari birbirinden farkli " << Common.DigitCount << " basamakli pozitif bir sayi tunuz." << endl;
			Game_state = start;
		}
		else
		{
			cout << "gecersiz sayi girildi."<<endl;
			Game_state = initialize;
		}
		break;

		case start:
			values.Holding_Number = Generate_Number(Common.DigitCount, &values.Holding_Array[0]);
			cout << "tahmin ettiginiz sayiyi giriniz."<<endl;
			user.Estimating_Number = Generate_Number(Common.DigitCount, &user.Estimating_Array[0]);
			for (size_t i = 0; i < 10; i++)
			{
				for (size_t j = 0; j < 10; j++)
				{
					tablo[i][j] = 1;
				}
				
			}
			Game_state = human;
			break;
		case human:
			cout << endl << endl;
			for (size_t i = 0; i < 10; i++)
			{
				values.dogruluk_tablosu[i] = 0;
				values.Estimating_Array[i] = 0;			
			}			
			cout << "Rakamlari birbirinden farkli " << Common.DigitCount << " basamakli pozitif bir sayi giriniz." << endl;
			cin >> Common.data ;
			if (Kurallara_Uygunmu(Common.data, Common.DigitCount))
			{
				Tahmin_Kontrolu();
				if (SonucGoster()) Game_state = initialize;
				else Game_state = computer;
			}
			else
			{
				cout << " kurallara uygun sayi girmediniz !!!" << endl;
				Game_state = human;
			}
			
			break;

		case computer:

			cout << endl << endl;			
			cout << "bilgisayarin tahmin ettigi sayi : " << user.Estimating_Number << endl;
			cout << "bilgisayarin tahmin ettigi degere gore ipucu verin.Ipucu rakamlara sirasiyla girilecektir." << endl;
			cout << "Basamak ve Sayi degeri dogru olanlar icin : 1" << endl;
			cout << "Sadece sayi degeri dogru olanlar icin : -1" << endl;
			cout << "tamami yanlis olanlar icin :0 degerlerini giriniz." << endl;					
			if (IpucuAl())
			{
				Game_state = initialize;
				cout<<endl<<endl << "oyunu bilgisayar kazandi." << endl<<endl;
				cout << "oyun sonlandi" << endl<<endl;
			}
			else
			{
				Game_state = human;
				cout << "Tahmin sirasi kullanicida" << endl;
			}
			SayiTahminEt();
			

			break;

	default:
		break;


	}

}
int main()
{
	srand(time(0));
	while (true)
	{

		Game();

	}
}

