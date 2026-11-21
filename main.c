#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

struct Hasta {
    int id;
    char ad[50];
    char telefon[20];
    char bolum[30];
};

struct Randevu {
    int hastaId;
    char tarih[20];
    char saat[10];
    double ucret;
    int tamamlandi;
};

struct Hasta hastalar[MAX];
struct Randevu randevular[MAX];
int hastaSayisi = 0;
int randevuSayisi = 0;

/* ---------- Yardimci giris fonksiyonlari ---------- */

/* Bir satir okur, sondaki \n'i siler, ';' karakterini bosluga cevirir */
void satirOku(char *hedef, int boyut) {
    if (fgets(hedef, boyut, stdin) == NULL) {
        hedef[0] = '\0';
        return;
    }
    size_t n = strlen(hedef);
    if (n > 0 && hedef[n - 1] == '\n') {
        hedef[n - 1] = '\0';
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF); /* kalan fazlalik temizlenir */
    }
    for (int i = 0; hedef[i]; i++) {
        if (hedef[i] == ';') hedef[i] = ' ';
    }
}

/* Gecerli tam sayi alir; basarisizsa 0 doner */
int sayiOku(int *sonuc) {
    char tampon[64];
    if (fgets(tampon, sizeof(tampon), stdin) == NULL) return 0;
    return sscanf(tampon, "%d", sonuc) == 1;
}

int ondalikOku(double *sonuc) {
    char tampon[64];
    if (fgets(tampon, sizeof(tampon), stdin) == NULL) return 0;
    return sscanf(tampon, "%lf", sonuc) == 1;
}

/* GG/AA/YYYY bicimini kontrol eder */
int tarihGecerliMi(const char *t) {
    int g, a, y;
    char fazla;
    if (sscanf(t, "%d/%d/%d%c", &g, &a, &y, &fazla) != 3) return 0;
    if (g < 1 || g > 31 || a < 1 || a > 12 || y < 1900 || y > 2100) return 0;
    return 1;
}

/* Buyuk-kucuk harf ayirmadan arama icin kucuk harfe cevirir */
void kucukHarf(const char *kaynak, char *hedef, int boyut) {
    int i;
    for (i = 0; kaynak[i] && i < boyut - 1; i++) {
        hedef[i] = (char)tolower((unsigned char)kaynak[i]);
    }
    hedef[i] = '\0';
}

const char *hastaAdiBul(int id) {
    for (int i = 0; i < hastaSayisi; i++) {
        if (hastalar[i].id == id) return hastalar[i].ad;
    }
    return "Bilinmiyor";
}

/* ---------- Dosya islemleri ---------- */

void dosyayaKaydet() {
    FILE *f = fopen("hastane.txt", "w");
    if (f == NULL) {
        printf("Uyari: hastane.txt dosyasi yazilamadi!\n");
        return;
    }
    fprintf(f, "Hastalar:%d\n", hastaSayisi);
    for (int i = 0; i < hastaSayisi; i++) {
        fprintf(f, "%d;%s;%s;%s\n", hastalar[i].id, hastalar[i].ad,
                hastalar[i].telefon, hastalar[i].bolum);
    }
    fprintf(f, "Randevular:%d\n", randevuSayisi);
    for (int i = 0; i < randevuSayisi; i++) {
        fprintf(f, "%d;%s;%s;%.2f;%d\n", randevular[i].hastaId,
                randevular[i].tarih, randevular[i].saat,
                randevular[i].ucret, randevular[i].tamamlandi);
    }
    fclose(f);
}

void dosyadanOku() {
    FILE *f = fopen("hastane.txt", "r");
    if (f == NULL) return; /* ilk calistirmada dosya yoktur */

    if (fscanf(f, "Hastalar:%d\n", &hastaSayisi) != 1) {
        hastaSayisi = 0;
        fclose(f);
        return;
    }
    if (hastaSayisi > MAX) hastaSayisi = MAX;

    for (int i = 0; i < hastaSayisi; i++) {
        if (fscanf(f, "%d;%49[^;];%19[^;];%29[^\n]\n", &hastalar[i].id,
                   hastalar[i].ad, hastalar[i].telefon, hastalar[i].bolum) != 4) {
            hastaSayisi = i;
            break;
        }
    }

    if (fscanf(f, "Randevular:%d\n", &randevuSayisi) != 1) {
        randevuSayisi = 0;
        fclose(f);
        return;
    }
    if (randevuSayisi > MAX) randevuSayisi = MAX;

    for (int i = 0; i < randevuSayisi; i++) {
        if (fscanf(f, "%d;%19[^;];%9[^;];%lf;%d\n", &randevular[i].hastaId,
                   randevular[i].tarih, randevular[i].saat,
                   &randevular[i].ucret, &randevular[i].tamamlandi) != 5) {
            randevuSayisi = i;
            break;
        }
    }
    fclose(f);
}

/* ---------- Ana islemler ---------- */

void yeniHastaKaydi() {
    if (hastaSayisi >= MAX) {
        printf("Hasta limiti doldu!\n");
        return;
    }
    printf("\n--- Yeni Hasta Kaydi ---\n");
    struct Hasta *h = &hastalar[hastaSayisi];

    printf("Ad Soyad: ");
    satirOku(h->ad, sizeof(h->ad));
    if (strlen(h->ad) == 0) {
        printf("Ad bos olamaz!\n");
        return;
    }
    printf("Telefon: ");
    satirOku(h->telefon, sizeof(h->telefon));
    printf("Bolum: ");
    satirOku(h->bolum, sizeof(h->bolum));

    h->id = (hastaSayisi == 0) ? 1 : hastalar[hastaSayisi - 1].id + 1;
    printf("Kaydedildi! ID: %d\n", h->id);
    hastaSayisi++;
    dosyayaKaydet();
}

void randevuOlustur() {
    if (randevuSayisi >= MAX) {
        printf("Randevu limiti doldu!\n");
        return;
    }
    int id;
    printf("\n--- Randevu Olusturma ---\n");
    printf("Hasta ID gir: ");
    if (!sayiOku(&id)) {
        printf("Gecersiz ID!\n");
        return;
    }

    int bulundu = 0;
    for (int i = 0; i < hastaSayisi; i++) {
        if (hastalar[i].id == id) {
            bulundu = 1;
            break;
        }
    }
    if (!bulundu) {
        printf("Hasta bulunamadi!\n");
        return;
    }

    struct Randevu yeni;
    yeni.hastaId = id;

    printf("Tarih (GG/AA/YYYY): ");
    satirOku(yeni.tarih, sizeof(yeni.tarih));
    if (!tarihGecerliMi(yeni.tarih)) {
        printf("Gecersiz tarih formati!\n");
        return;
    }

    printf("Saat (SS:DD): ");
    satirOku(yeni.saat, sizeof(yeni.saat));
    if (strlen(yeni.saat) == 0) {
        printf("Saat bos olamaz!\n");
        return;
    }

    /* Ayni tarih ve saatte baska randevu var mi? */
    for (int i = 0; i < randevuSayisi; i++) {
        if (strcmp(randevular[i].tarih, yeni.tarih) == 0 &&
            strcmp(randevular[i].saat, yeni.saat) == 0) {
            printf("Bu tarih ve saatte baska bir randevu var!\n");
            return;
        }
    }

    printf("Ucret: ");
    if (!ondalikOku(&yeni.ucret) || yeni.ucret < 0) {
        printf("Gecersiz ucret!\n");
        return;
    }
    yeni.tamamlandi = 0;

    randevular[randevuSayisi] = yeni;
    randevuSayisi++;
    printf("Randevu olusturuldu!\n");
    dosyayaKaydet();
}

void randevuListele() {
    printf("\n--- Randevu Listesi ---\n");
    if (randevuSayisi == 0) {
        printf("Liste bos!\n");
        return;
    }
    for (int i = 0; i < randevuSayisi; i++) {
        printf("%d) %s - Tarih:%s Saat:%s Ucret:%.2f Durum:%s\n",
               i + 1, hastaAdiBul(randevular[i].hastaId),
               randevular[i].tarih, randevular[i].saat, randevular[i].ucret,
               randevular[i].tamamlandi ? "Tamamlandi" : "Bekliyor");
    }
}

void faturaKes() {
    printf("\n--- Fatura Kesme ve Islem Tamamlama ---\n");
    if (randevuSayisi == 0) {
        printf("Randevu yok!\n");
        return;
    }
    randevuListele();

    int secim;
    printf("Tamamlanacak randevu no: ");
    if (!sayiOku(&secim)) {
        printf("Gecersiz giris!\n");
        return;
    }
    secim--;
    if (secim < 0 || secim >= randevuSayisi) {
        printf("Gecersiz!\n");
        return;
    }
    struct Randevu *r = &randevular[secim];
    if (r->tamamlandi) {
        printf("Zaten tamamlandi!\n");
        return;
    }
    r->tamamlandi = 1;

    printf("\n===== FATURA =====\n");
    printf("Hasta : %s\n", hastaAdiBul(r->hastaId));
    printf("Tarih : %s  Saat: %s\n", r->tarih, r->saat);
    printf("Tutar : %.2f TL\n", r->ucret);
    printf("Odeme alindi, islem tamamlandi!\n");
    printf("==================\n");

    /* Faturayi dosyaya da ekle */
    FILE *f = fopen("fatura.txt", "a");
    if (f != NULL) {
        fprintf(f, "Hasta:%s | Tarih:%s | Saat:%s | Tutar:%.2f TL\n",
                hastaAdiBul(r->hastaId), r->tarih, r->saat, r->ucret);
        fclose(f);
    }
    dosyayaKaydet();
}

void hastaArama() {
    char aranan[50], arananK[50], adK[50];
    printf("\n--- Hasta Arama ---\n");
    printf("Ad gir: ");
    satirOku(aranan, sizeof(aranan));
    if (strlen(aranan) == 0) {
        printf("Arama metni bos!\n");
        return;
    }
    kucukHarf(aranan, arananK, sizeof(arananK));

    int bulundu = 0;
    for (int i = 0; i < hastaSayisi; i++) {
        kucukHarf(hastalar[i].ad, adK, sizeof(adK));
        if (strstr(adK, arananK) != NULL) {
            printf("ID:%d Ad:%s Tel:%s Bolum:%s\n", hastalar[i].id,
                   hastalar[i].ad, hastalar[i].telefon, hastalar[i].bolum);
            bulundu = 1;
        }
    }
    if (!bulundu) printf("Hasta bulunamadi!\n");
}

void gunlukRapor() {
    char tarih[20];
    printf("\n--- Gunluk Raporlama (Bonus) ---\n");
    printf("Rapor tarihi (GG/AA/YYYY): ");
    satirOku(tarih, sizeof(tarih));
    if (!tarihGecerliMi(tarih)) {
        printf("Gecersiz tarih formati!\n");
        return;
    }

    double ciro = 0;
    int toplam = 0, tamamlanan = 0, bekleyen = 0;
    for (int i = 0; i < randevuSayisi; i++) {
        if (strcmp(randevular[i].tarih, tarih) != 0) continue;
        toplam++;
        if (randevular[i].tamamlandi) {
            ciro += randevular[i].ucret;
            tamamlanan++;
        } else {
            bekleyen++;
        }
    }

    printf("Tarih: %s\n", tarih);
    printf("Toplam Randevu: %d\n", toplam);
    printf("Tamamlanan: %d\n", tamamlanan);
    printf("Bekleyen: %d\n", bekleyen);
    printf("Gunluk Ciro: %.2f TL\n", ciro);

    /* Raporu dosyaya da yaz */
    FILE *f = fopen("rapor.txt", "a");
    if (f != NULL) {
        fprintf(f, "Tarih:%s | Toplam:%d | Tamamlanan:%d | Bekleyen:%d | Ciro:%.2f TL\n",
                tarih, toplam, tamamlanan, bekleyen, ciro);
        fclose(f);
        printf("Rapor rapor.txt dosyasina kaydedildi.\n");
    }
}

int main() {
    dosyadanOku();
    int secim;
    while (1) {
        printf("\n===== MUSTERI VE RANDEVU YONETIM SISTEMI =====\n");
        printf("1) Yeni Hasta Kaydi\n");
        printf("2) Randevu Olusturma\n");
        printf("3) Randevu Listesini Goruntuleme\n");
        printf("4) Fatura Kesme ve Islem Tamamlama\n");
        printf("5) Hasta Arama\n");
        printf("6) Cikis\n");
        printf("7) Gunluk Raporlama (Bonus)\n");
        printf("Seciminiz: ");

        if (!sayiOku(&secim)) {
            printf("Gecersiz secim!\n");
            if (feof(stdin)) break;
            continue;
        }

        if (secim == 1) yeniHastaKaydi();
        else if (secim == 2) randevuOlustur();
        else if (secim == 3) randevuListele();
        else if (secim == 4) faturaKes();
        else if (secim == 5) hastaArama();
        else if (secim == 6) {
            printf("Cikis...\n");
            break;
        }
        else if (secim == 7) gunlukRapor();
        else printf("Gecersiz secim!\n");
    }
    return 0;
}
