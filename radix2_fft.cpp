#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

class ComplexNum
{
private:
    float re, im;

public:
    ComplexNum(float a = 0.0f, float b = 0.0f) : re(a), im(b) {}

    float real() const { return re; }
    float imag() const { return im; }

    // Karmaşık sayının büyüklüğünü (genliğini) hesaplar
    float magnitude() const
    {
        return std::sqrt(re * re + im * im);
    }

    ComplexNum operator+(const ComplexNum &n) const
    { // Dışarıdan gelen n objesini kopyalama, sadece referansını (adresini) al (&)
        return ComplexNum(re + n.re, im + n.im);
    }

    ComplexNum operator*(const ComplexNum &n) const
    {
        return ComplexNum((re * n.re) - (im * n.im), (re * n.im) + (im * n.re));
    }

    // Karmaşık sayılarda çıkarma: (a + bi) - (c + di) = (a-c) + (b-d)i
    ComplexNum operator-(const ComplexNum &n) const
    {
        return ComplexNum(re - n.re, im - n.im);
    }

    // Karmaşık sayının eşleniğini (conjugate) alır: a + bi -> a - bi
    ComplexNum conjugate() const
    {
        return ComplexNum(re, -im);
    }
};

class SignalVector
{
private:
    std::vector<ComplexNum> sinyal_verisi; // sinyal örneklerini tutacak olan vektör

public:
    void push(const ComplexNum &c)
    { // dışarıdan gelen bir karmaşık sayıyı vektörün sonuna ekleyecek bir fonk
        sinyal_verisi.push_back(c);
    }
    size_t size() const
    { // size_t bellekteki nesne sayılarını ve indeksleri ifade etmek için standart ve en güvenli veri tipi
        return sinyal_verisi.size();
    }

    ComplexNum &operator[](size_t indeks)
    { // Bu sayede main içinde "benimSinyalim[5]" yazarak 5. elemana erişebileceğiz
        return sinyal_verisi[indeks];
    }

    const ComplexNum &operator[](size_t indeks) const
    { // Güvenlik için bir de sadece okuma (read-only) yapan const versiyonunu ekleyelim:
        return sinyal_verisi[indeks];
    }

    SignalVector operator+(const SignalVector &diger) const
    {
        if (size() != diger.size())
        {
            std::cout << "Hata: Vektor boyutlari esit degil!";
            return SignalVector();
        }

        else
        {
            SignalVector sonuc;
            for (size_t i = 0; i < size(); i++)
            {
                sonuc.push(sinyal_verisi[i] + diger[i]);
            }
            return sonuc;
        }
    }

    SignalVector operator*(const SignalVector &diger) const
    {
        if (size() != diger.size())
        {
            std::cout << "Hata: Vektor boyutlari esit degil!";
            return SignalVector();
        }

        else
        {
            SignalVector sonuc;
            for (size_t i = 0; i < size(); i++)
            {
                sonuc.push(sinyal_verisi[i] * diger[i]);
            }
            return sonuc;
        }
    }
};

class FFT
{
public:
    static void butterfly(ComplexNum &a, ComplexNum &b, const ComplexNum &twiddle)
    { // Amacı işlemciyi (CPU) kurtarmak. Kelebek algoritması bu simetriyi kullanarak, tek bir çarpma işleminin sonucunu dizideki iki farklı elemana aynı anda uygular. Yani matematiksel bir ihtiyacı değil, donanımsal bir optimizasyonu (çarpım sayısını yarıya indirmeyi) temsil eder.
        ComplexNum temp = b * twiddle;
        b = a - temp;
        a = a + temp;
    }

    static ComplexNum computeTwiddle(int k, int N)
    {
        float aci = -2.0f * M_PI * k / N;
        return ComplexNum(std::cos(aci), std::sin(aci));
    }

    static size_t bitReversal(size_t x, int log2N)
    {
        size_t sonuc = 0;

        for (int i = 0; i < log2N; i++)
        {
            if (x & (1 << i))
            {
                sonuc |= (1 << ((log2N - 1) - i));
            }
        }
        return sonuc;
    }

    static SignalVector computeFFT(const SignalVector &sinyal)
    {
        size_t N = sinyal.size();
        int log2N = std::log2(N);
        SignalVector sonuc;

        for (int i = 0; i < N; i++)
        {
            sonuc.push(sinyal[bitReversal(i, log2N)]);
        }

        for (int s = 1; s <= log2N; s++)
        {
            int m = 1 << s;
            int m2 = m / 2;
            for (size_t k = 0; k < N; k += m)
            {
                for (int j = 0; j < m2; j++)
                {
                    ComplexNum w = computeTwiddle(j, m);
                    butterfly(sonuc[k + j], sonuc[k + j + m2], w);
                }
            }
        }
        return sonuc;
    }

    static SignalVector computeIFFT(const SignalVector &frekanslar)
    {
        size_t N = frekanslar.size();
        SignalVector temp;

        // 1. ADIM: Frekans verilerinin eşleniğini al
        for (size_t i = 0; i < N; i++)
        {
            temp.push(frekanslar[i].conjugate());
        }

        // 2. ADIM: Eşlenikleri alınmış veriye normal FFT uygula
        SignalVector sonuc = computeFFT(temp);

        // 3. ADIM: Çıkan sonucun tekrar eşleniğini al ve N'e böl
        SignalVector zaman_duzlemi;
        for (size_t i = 0; i < N; i++)
        {
            ComplexNum c = sonuc[i].conjugate();
            zaman_duzlemi.push(ComplexNum(c.real() / N, c.imag() / N));
        }

        return zaman_duzlemi;
    }
};
