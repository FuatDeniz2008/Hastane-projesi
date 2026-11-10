#include <stdio.h>
#include <string.h>

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

void dosyayaKaydet() {
    FILE *f = fopen("hastane.txt", "w");
    if(f == NULL) return;
    fprintf(f, "Hastalar:%d\n", hastaSayisi);
    for(int i=0; i<hastaSayisi; i++){
        fprintf(f, "%d %s %s %s\n", hastalar[i].id, hastalar[i].ad, hastalar[i].telefon, hastalar[i].bolum);
    }
    fprintf(f, "Randevular:%d\n", randevuSayisi);
    for(int i=0; i<randevuSayisi; i++){
        fprintf(f, "%d %s %s %.2f %d\n", randevular[i].hastaId, randevular[i].tarih, randevular[i].saat, randevular[i].ucret, randevular[i].tamamlandi);
    }
    fclose(f);
}

void dosyadanOku() {
    FILE *f = fopen("hastane.txt", "r");
    if(f == NULL) return;
    fscanf(f, "Hastalar:%d\n", &hastaSayisi);
    for(int i=0; i<hastaSayisi; i++){
        fscanf(f, "%d %s %s %s\n", &hastalar[i].id, hastalar[i].ad, hastalar[i].telefon, hastalar[i].bolum);
    }
    fscanf(f, "Randevular:%d\n", &randevuSayisi);
    for(int i=0; i<randevuSayisi; i++){
        fscanf(f, "%d %s %s %lf %d\n", &randevular[i].hastaId, randevular[i].tarih, randevular[i].saat, &randevular[i].ucret, &randevular[i].tamamlandi);
    }
    fclose(f);
}

void yeniHastaKaydi() {
    if(hastaSayisi >= MAX) { printf("Hasta limiti doldu!\n"); return; }
    printf("\n--- Yeni Hasta Kaydi ---\n");
    hastalar[hastaSayisi].id = hastaSayisi + 1;
    printf("Ad Soyad: ");
    scanf(" %[^\n]", hastalar[hastaSayisi].ad);
    printf("Telefon: ");
    scanf("%s", hastalar[hastaSayisi].telefon);
    printf("Bolum: ");
    scanf(" %[^\n]", hastalar[hastaSayisi].bolum);
    printf("Kaydedildi! ID: %d\n", hastalar[hastaSayisi].id);
    hastaSayisi++;
    dosyayaKaydet();
}

void randevuOlustur() {
    if(randevuSayisi >= MAX) { printf("Randevu limiti doldu!\n"); return; }
    int id;
    printf("\n--- Randevu Olusturma ---\n");
    printf("Hasta ID gir: ");
    scanf("%d", &id);
    int bulundu = 0;
    for(int i=0; i<hastaSayisi; i++){
        if(hastalar[i].id == id) { bulundu = 1; break; }
    }
    if(!bulundu) { printf("Hasta bulunamadi!\n"); return; }
    randevular[randevuSayisi].hastaId = id;
    printf("Tarih (GG/AA/YYYY): ");
    scanf("%s", randevular[randevuSayisi].tarih);
    printf("Saat: ");
    scanf("%s", randevular[randevuSayisi].saat);
    printf("Ucret: ");
    scanf("%lf", &randevular[randevuSayisi].ucret);
    randevular[randevuSayisi].tamamlandi = 0;
    printf("Randevu olusturuldu!\n");
    randevuSayisi++;
    dosyayaKaydet();
}

void randevuListele() {
    printf("\n--- Randevu Listesi ---\n");
    if(randevuSayisi==0) { printf("Liste bos!\n"); return; }
    for(int i=0; i<randevuSayisi; i++){
        char hastaAdi[50] = "Bilinmiyor";
        for(int j=0; j<hastaSayisi; j++){
            if(hastalar[j].id == randevular[i].hastaId){
                strcpy(hastaAdi, hastalar[j].ad);
                break;
            }
        }
        printf("%d) %s - Tarih:%s Saat:%s Ucret:%.2f Durum:%s\n",
            i+1, hastaAdi, randevular[i].tarih, randevular[i].saat,
            randevular[i].ucret, randevular[i].tamamlandi ? "Tamamlandi" : "Bekliyor");
    }
}

void faturaKes() {
    printf("\n--- Fatura Kesme ---\n");
    randevuListele();
    int secim;
    printf("Tamamlanacak randevu no: ");
    scanf("%d", &secim);
    secim--;
    if(secim <0 || secim >= randevuSayisi) { printf("Gecersiz!\n"); return; }
    if(randevular[secim].tamamlandi) { printf("Zaten tamamlandi!\n"); return; }
    randevular[secim].tamamlandi = 1;
    printf("\n===== FATURA =====\n");
    printf("Tutar: %.2f TL\n", randevular[secim].ucret);
    printf("Odeme alindi!\n");
    printf("==================\n");
    dosyayaKaydet();
}

void hastaArama() {
    char aranan[50];
    printf("\n--- Hasta Arama ---\n");
    printf("Ad gir: ");
    scanf(" %[^\n]", aranan);
    int bulundu = 0;
    for(int i=0; i<hastaSayisi; i++){
        if(strstr(hastalar[i].ad, aranan) != NULL){
            printf("ID:%d Ad:%s Tel:%s Bolum:%s\n", hastalar[i].id, hastalar[i].ad, hastalar[i].telefon, hastalar[i].bolum);
            bulundu = 1;
        }
    }
    if(!bulundu) printf("Hasta bulunamadi!\n");
}

void gunlukRapor() {
    printf("\n--- Gunluk Raporlama (Bonus) ---\n");
    double toplam = 0;
    int tamamlanan = 0, bekleyen = 0;
    for(int i=0; i<randevuSayisi; i++){
        if(randevular[i].tamamlandi){ toplam += randevular[i].ucret; tamamlanan++; }
        else { bekleyen++; }
    }
    printf("Toplam Randevu: %d\n", randevuSayisi);
    printf("Tamamlanan: %d\n", tamamlanan);
    printf("Bekleyen: %d\n", bekleyen);
    printf("Gunluk Ciro: %.2f TL\n", toplam);
}

int main() {
    dosyadanOku();
    int secim;
    while(1){
        printf("\n===== MUSTERI VE RANDEVU YONETIM SISTEMI =====\n");
        printf("1) Yeni Hasta Kaydi\n");
        printf("2) Randevu Olusturma\n");
        printf("3) Randevu Listesini Goruntuleme\n");
        printf("4) Fatura Kesme ve Islem Tamamlama\n");
        printf("5) Hasta Arama\n");
        printf("6) Gunluk Raporlama (Bonus)\n");
        printf("7) Cikis\n");
        printf("Seciminiz: ");
        scanf("%d", &secim);
        if(secim==1) yeniHastaKaydi();
        else if(secim==2) randevuOlustur();
        else if(secim==3) randevuListele();
        else if(secim==4) faturaKes();
        else if(secim==5) hastaArama();
        else if(secim==6) gunlukRapor();
        else if(secim==7) { printf("Cikis...\n"); break; }
        else printf("Gecersiz secim!\n");
    }
    return 0;
}
