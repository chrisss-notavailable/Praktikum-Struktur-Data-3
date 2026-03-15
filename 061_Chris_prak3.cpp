#include<iostream>
#include<fstream>
#include<sstream>
#include <algorithm>
#include <cctype>
using namespace std ;

string toLowerCase(string str){
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}


// STRUCT PROGRAM

struct servis {
    string masuk;
    string model;
    string merek;
    string kendala;
    string montir;
    string namac;
    long long noc;
    servis* next;
};

struct cust{
    string nama;
    int umur;
    string gender;
    long long no;
    string alamat;
    cust* next;
    cust* prev ;
};

struct Datamontir{
    string nama;
    Datamontir* next;
};


// PROTOTYPE FUNGSI

void simpanCustomer();
void simpanServis();
void simpanHistory(servis* simpan);
void loadCustomer();
void loadAntrian();
void loadHistory();
void dataCust();
void tambahSS();
void dataSS();
void servisSelesai();
void riwayatServis();
void montir();
void menuServis();
void menuAdmin();
void masukServis(servis* baru);

void batalkanServis(string nama,long long no);
void undoPembatalan();
void menuCustomer(string nama,long long no);
void pushCancel(servis* data);
servis* popCancel();
void tambahMontir();
bool cekMontir(string nama);
void tampilMontir();
void simpanMontir();
void loadMontir();
void bookingServisCustomer(string nama, long long no);
void loadStack();
void simpanStack();

// VAR GLOBAL

cust* headCust = NULL;
// servis* semuaServis = NULL ;
servis* frontServis = NULL ;
servis* rearServis = NULL;
servis* selesaiServis = NULL;
Datamontir* headMontir = NULL; 

// STACK PEMBATALAN SERVIS
servis* topCancel = NULL; // LIFO

// FUNNGSI DASAR 

// PUSH STACK PEMBATALAN - tambah
void pushCancel(servis* data){
    data->next = topCancel;
    topCancel = data;
}

// POP STACK PEMBATALAN - ambil 
servis* popCancel(){

    if(topCancel == NULL)
        return NULL;

    servis* ambil = topCancel;

    topCancel = topCancel->next;

    ambil->next = NULL;

    return ambil;
}

// BATALKAN SERVIS
void batalkanServis(string nama,long long no){

    servis* scanner = frontServis;
    servis* prev = NULL;

    servis* daftar[100];
    int jumlah = 0;

    while(scanner != NULL){

        if(toLowerCase(scanner->namac) == toLowerCase(nama) || scanner->noc == no){

            cout << jumlah+1 << ". ";
            cout << scanner->model << " - " << scanner->kendala << endl;

            daftar[jumlah] = scanner;
            jumlah++;
        }

        scanner = scanner->next;
    }

    if(jumlah == 0){
        cout << "Tidak ada servis\n";
        system("pause");
        return;
    }

    int pilih;

    cout << "Pilih Mobil yang ingin dibatalkan: ";
    cin >> pilih;

    servis* target = daftar[pilih-1];

    scanner = frontServis;
    prev = NULL;

    while(scanner != NULL){

        if(scanner == target)
            break;

        prev = scanner;
        scanner = scanner->next;
    }

    if(prev == NULL){
        frontServis = target->next;
    }
    else{
        prev->next = target->next;
    }

    if(target == rearServis){
        rearServis = prev;
    }

    pushCancel(target);
    simpanStack();

    cout << "Servis " << target->model << " telah dibatalkan\n";

    simpanServis();

    system("pause");
}

// UNDO PEMBATALAN
void undoPembatalan(){

    if(topCancel == NULL){
        cout << "Tidak ada pembatalan servis\n";
        system("pause");
        return;
    }

    cout << "\n====== Booking Kembali Service ======\n";

    servis* daftar[100];
    int jumlah = 0;

    servis* scan = topCancel;

    while(scan != NULL){
        cout << jumlah+1 << ". "
            << scan->model << " | "
            << scan->merek << " | "
            << scan->kendala << " | "
            << scan->montir << endl;

        daftar[jumlah] = scan;
        jumlah++;

        scan = scan->next;
    }

    int pilih;
    cout << "\nPilih servis yang ingin dibooking kembali: ";
    cin >> pilih;

    if(pilih < 1 || pilih > jumlah){
        system("pause");
        return;
    }

    servis* target = daftar[pilih-1];

    // Hapus dari stack
    servis* prev = NULL;
    scan = topCancel;

    while(scan != NULL){

        if(scan == target)
            break;

        prev = scan;
        scan = scan->next;
    }

    if(prev == NULL)
        topCancel = target->next;
    else
        prev->next = target->next;

    target->next = NULL;

    string jawab;

    cout << "Apakah anda ingin membooking kembali servis ini? (yes/no): ";
    cin >> jawab;

    if(jawab == "yes"){

        string tanggalBaru;

        cout << "Tanggal Lama: " << target->masuk << endl;
        cout << "Tanggal Baru (- jika tidak): ";
        cin >> tanggalBaru;

        if(tanggalBaru != "-")
            target->masuk = tanggalBaru;

        masukServis(target);

        cout << "Servis berhasil dibooking kembali\n";

        simpanServis();
    }

    simpanStack();

    system("pause");
}

cust* cariCust (string nama) {

    if(headCust == NULL)
        return NULL;

    cust* bantu = headCust;

    do {
        if (bantu->nama == nama)
            return bantu;

        bantu = bantu->next;

    } while(bantu != headCust);

    return NULL;
}

void masukServis (servis* baru){
    if(frontServis == NULL){
        frontServis = baru ;
        rearServis = baru;
    }

    else{
        rearServis -> next = baru;
        rearServis = baru ;
    }
}

servis* keluarServis (){
    
    if(frontServis == NULL ){
        return NULL;
    }
    servis* keluar = frontServis ;
    frontServis = frontServis -> next ;

    if(frontServis == NULL){
        rearServis = NULL;
    }
    keluar -> next = NULL;
    return keluar;
} 

void tambahServisFile(string masuk,string model, string merek,string kendala, string montir,string namac, long long noc)
{
    servis* baru = new servis;
    baru->masuk = masuk;
    baru->model = model;
    baru->merek = merek;
    baru->kendala = kendala;
    baru->montir = montir;
    baru->namac = namac;
    baru->noc = noc;
    baru->next = NULL;

    masukServis(baru);
}

bool cekMontir(string nama){

    Datamontir* scanner = headMontir;

    while(scanner != NULL){

        if(toLowerCase(scanner->nama) == toLowerCase(nama))
            return true;

        scanner = scanner->next;
    }

    return false;
}
// OPERASI PROGRAM

void antrianCustomer(string nama){

    if(frontServis == NULL){
        cout << "Tidak ada servis dalam antrian\n";
        system("pause");
        return;
    }

    cout << "\n====== All Services ======\n";

    servis* bantu = frontServis;
    int antrian = 1;

    int antrianUser[100];
    string mobilUser[100];
    int jumlah = 0;

    while(bantu != NULL){

        cout << "-------------------------\n";
        cout << "Model Mobil: " << bantu->model << endl;
        cout << "Merek Mobil: " << bantu->merek << endl;
        cout << "Kendala: " << bantu->kendala << endl;
        cout << "Montir: " << bantu->montir << endl;
        cout << "Nama Pelanggan: " << bantu->namac << endl;
        cout << "No Telp Pelanggan: " << bantu->noc << endl;
        cout << "-------------------------\n";

        if(toLowerCase(bantu->namac) == toLowerCase(nama)){
            antrianUser[jumlah] = antrian;
            mobilUser[jumlah] = bantu->model;
            jumlah++;
        }

        bantu = bantu->next;
        antrian++;
    }

    if(jumlah == 0){
        cout << "Anda tidak memiliki servis dalam antrian\n";
    }
    else{
        for(int i=0;i<jumlah;i++){
            cout << "Servis " << mobilUser[i]
                << " Anda berada di antrian ke-"
                << antrianUser[i] << endl;
        }
    }

    system("pause");
}


void simpanStack(){

    ofstream file("Booking.txt");

    servis* bantu = topCancel;

    while(bantu != NULL){

        file << bantu->masuk << "|"
            << bantu->model << "|"
            << bantu->merek << "|"
            << bantu->kendala << "|"
            << bantu->montir << "|"
            << bantu->namac << "|"
            << bantu->noc << endl;

        bantu = bantu->next;
    }

    file.close();
}

void loadStack(){

    ifstream file("Booking.txt");

    if(!file.is_open())
        return;

    string line;

    while(getline(file,line)){

        stringstream ss(line);

        string masuk,model,merek,kendala,montir,namac,noStr;

        getline(ss,masuk,'|');
        getline(ss,model,'|');
        getline(ss,merek,'|');
        getline(ss,kendala,'|');
        getline(ss,montir,'|');
        getline(ss,namac,'|');
        getline(ss,noStr);

        servis* baru = new servis;

        baru->masuk = masuk;
        baru->model = model;
        baru->merek = merek;
        baru->kendala = kendala;
        baru->montir = montir;
        baru->namac = namac;
        baru->noc = stoll(noStr);

        baru->next = topCancel;
        topCancel = baru;
    }

    file.close();
}

void bookingServisCustomer(string nama, long long no) {
    cin.ignore();

    string model, merek, kendala, masuk;
    string montir;

    cout << "\n====== New Service ======\n";

    cout << "Model Mobil: ";
    getline(cin, model);

    cout << "Merek Mobil: ";
    getline(cin, merek);

    cout << "Kendala: ";
    getline(cin, kendala);

    cout << "Tanggal Masuk Bengkel: ";
    getline(cin, masuk);

    // NYARI MOBILL
    servis* cari = frontServis; 
    servis* servisLama = NULL;

    int nomor = 1;
    int nomorAntrian = -1;

    while (cari != NULL) {
        if (toLowerCase(cari->namac) == toLowerCase(nama) && toLowerCase(cari->model) == toLowerCase(model) && toLowerCase(cari->merek) == toLowerCase(merek)) {

            servisLama = cari;
            nomorAntrian = nomor;
            break;
        }

        cari = cari->next;
        nomor++;
    }

    // jika mobil sudah ada
    if (servisLama != NULL) {
        string jawab;
        cout << "\nMobil ini sudah dibengkel anda ingin menambahkan kendala ini kepada servis tersebut (yes/no): ";
        cin >> jawab;

        if (jawab == "yes") {
            servisLama->kendala += " | " + kendala;
            cout << "\n*Kendala sudah diupdate, nomor antrian anda adalah: "
                << nomorAntrian << "*\n";
            simpanServis();
        }

        system("pause");
        return;
    }

    cout << "\nPilih Montir\n";
    tampilMontir();

    int pilih;
    cout << "Pilihan: ";
    cin >> pilih;
    cin.ignore();

    Datamontir* mntr = headMontir;
    int i = 1;

    while (mntr != NULL) {
        if (i == pilih) {
            montir = mntr ->nama;
            break;
        }
        mntr = mntr ->next;
        i++;
    }

    // buat servis baru
    servis* baru = new servis;
    baru->model = model;
    baru->merek = merek;
    baru->kendala = kendala;
    baru->masuk = masuk;
    baru->montir = montir;
    baru->namac = nama;
    baru->noc = no;
    baru->next = NULL;

    masukServis(baru);

    // cari nomor antrian
    int antrian = 1;
    cari = frontServis; // Menggunakan nama cari kembali

    while (cari != NULL) {
        if (cari == baru)
            break;

        cari = cari->next;
        antrian++;
    }

    cout << "\n*Servis sudah tercatat nomor antrian anda adalah: "
        << antrian << "*\n";

    simpanServis();
    system("pause");
}

void tampilMontir(){

    Datamontir* scanner = headMontir;
    int nomor = 1;

    while(scanner != NULL){

        cout << nomor << ". " << scanner->nama << endl;

        scanner = scanner->next;
        nomor++;
    }
}

void tambahMontir(){

    cin.ignore();

    string nama;

    cout << "\n====== Montir Baru ======\n";
    cout << "Masukkan Nama montir baru\n";
    cout << "Nama: ";
    getline(cin,nama);

    if(cekMontir(nama)){
        cout << "*" << nama << " sudah terdaftar sebagai montir*\n";
        system("pause");
        return;
    }

    Datamontir* baru = new Datamontir;

    baru->nama = nama;
    baru->next = NULL;

    if(headMontir == NULL){
        headMontir = baru;
    }
    else{
        Datamontir* scanner = headMontir;

        while(scanner->next != NULL){
            scanner = scanner->next;
        }

        scanner->next = baru;
    }

    simpanMontir();

    cout << "*" << nama << " telah ditambahkan ke daftar montir*\n";

    system("pause");
}

void simpanMontir(){

    ofstream file("Montir.txt");

    Datamontir* scanner = headMontir;

    while(scanner != NULL){

        file << scanner->nama << endl;

        scanner = scanner->next;
    }

    file.close();
}

void loadMontir(){

    ifstream file("Montir.txt");

    if(!file.is_open())
        return;

    string nama;

    while(getline(file,nama)){

        Datamontir* baru = new Datamontir;

        baru->nama = nama;
        baru->next = NULL;

        if(headMontir == NULL){
            headMontir = baru;
        }
        else{

            Datamontir* scanner = headMontir;

            while(scanner->next != NULL){
                scanner = scanner->next;
            }

            scanner->next = baru;
        }
    }

    file.close();
}

void tambahCust (string nama, int umur, string gender,long long no,string alamat){
    cust* baru = new cust;
    baru -> nama = nama ;
    baru -> umur = umur ;
    baru -> gender = gender ;
    baru -> no = no ;
    baru -> alamat = alamat ;
    baru -> next = NULL;
    baru -> prev = NULL;

    // if (headCust == NULL ){
    //     headCust = baru ;
    // }
    // else {
    //     cust* bantu = headCust;
    //     while (bantu -> next != NULL)
    //         bantu = bantu -> next;
    //     bantu -> next = baru;
    //     baru -> prev = bantu;
    // }
    if (headCust == NULL ){  // doubly circular
    headCust = baru;
    baru->next = baru;
    baru->prev = baru;
}
else {
    cust* tail = headCust->prev;

    tail->next = baru;
    baru->prev = tail;

    baru->next = headCust;
    headCust->prev = baru;
}
}


void tambahSS() {
    cin.ignore();
    string model,merek,kendala,montir,nama,masuk;

    cout << "Tanggal Masuk : ";
    getline(cin,masuk);
    cout << "Model mobil :";
    getline(cin,model);
    cout << "Merek mobil :";
    getline(cin,merek);
    cout << "Kendala :";
    getline(cin,kendala);
    cout << "Pilih Montir:\n";
    tampilMontir();

    int pilihMontir;
    cout << "Pilihan: ";
    cin >> pilihMontir;
    cin.ignore();
    Datamontir* scanner = headMontir;
    int nomor = 1;
    while(scanner != NULL){
        if(nomor == pilihMontir){
            montir = scanner->nama;
            break;
        }
        scanner = scanner->next;
        nomor++;
    }
    cout << "Nama Pelanggan :";
    getline (cin,nama);

    cust* pelanggan = cariCust(nama);

    if (pelanggan == NULL){
        long long no;
        int umur;
        string alamat, gender;

        cout <<"No HP :" ;
        cin >> no;
        cin.ignore();
        cout <<"Alamat :" ;
        getline(cin,alamat);
        cout <<"Umur :" ;
        cin >> umur;
        cin.ignore();
        cout <<"Gender : " ;
        getline(cin,gender);

        tambahCust (nama,umur,gender,no,alamat);
        pelanggan = cariCust(nama);
        
        cout << "Pelanggan baru berhasil terdaftar\n";
    }
    else{
        cout << "pelanggan sudah terdaftar\n";
    }

    servis* baru = new servis ;
    baru -> masuk = masuk;
    baru -> model = model;
    baru -> merek = merek;
    baru -> kendala = kendala;
    baru -> montir = montir;
    baru -> namac = pelanggan -> nama ;
    baru -> noc = pelanggan -> no ;
    baru -> next = NULL;

    masukServis(baru);

simpanCustomer();
simpanServis();

system("pause");
}

void dataSS(){

    if (frontServis == NULL) {
        cout << " Tidak ada servis ";
        system ("pause");
        return ;
    }

    cout <<"\n======== Data Service ========\n";

    servis* bantu = frontServis;
    int antri = 1;

    while(bantu != NULL){
        cout << "==============================================\n";
        cout << " Antrian ke : " << antri << endl;
        cout << " Tanggal Masuk Service :" << bantu -> masuk << endl;
        cout << " Model Mobil :" << bantu -> model << endl;
        cout << " Merek Mobil :" << bantu -> merek << endl;
        cout << " kendala : " << bantu -> kendala << endl;
        cout << " Montir : " << bantu -> montir << endl;
        cout << " Nama Pelanggan :" << bantu -> namac << endl;
        cout << " No Telp Pelanggan :" << bantu -> noc << endl;
        cout << "==============================================\n";
        bantu = bantu -> next ;
        antri ++;
    }

system("pause");
}

void dataCust () {
    if(headCust==NULL)
    return;
    
    cust* current = headCust;
    char menu;

    while (true) {
        cout << "\n========= Data Pelanggan =========\n";
        cout << "Nama : " << current -> nama << endl;
        cout << "Nomer Telepon : " << current -> no << endl;
        cout << "umur: " << current->umur << endl;
        cout << "Gender: " << current -> gender << endl;
        cout << "Alamat: " << current -> alamat << endl;

        cout <<"\n[N]EXT    [P]REVIOUS [E]EXIT\nPilihan :";
        cin >> menu;

        if (menu == 'N' || menu == 'n'){
            current = current-> next;
        }
        else if (menu == 'P' || menu == 'p'){
            current = current-> prev;
        }
        else if (menu == 'E' || menu == 'e'){
            break;
        }
    }
}


void semuaDataCust() {
if(headCust == NULL){
    cout << "Belum ada data pelanggan.\n";
    system("pause");
    return;
}

cout << "\n====== SEMUA PELANGGAN ======\n";

cust* cari = headCust;

do {
    cout << "-----------------------\n";
    cout << "Nama: " << cari->nama << endl;
    cout << "Nomor Telepon: " << cari->no << endl;
    cout << "Alamat: " << cari->alamat << endl;

    servis* cariAntri = frontServis;
    servis* servisTerakhir = NULL;


    while(cariAntri != NULL){
        if(toLowerCase(cariAntri->namac) == toLowerCase(cari->nama)){
            servisTerakhir = cariAntri;
        }
        cariAntri = cariAntri->next;
    }

    if(servisTerakhir == NULL){
        servis* cariHistory = selesaiServis;

        while(cariHistory != NULL){
            if(toLowerCase(cariHistory->namac) == toLowerCase(cari->nama)){
                servisTerakhir = cariHistory;
            }
            cariHistory = cariHistory->next;
        }
    }

    if(servisTerakhir != NULL){
        cout << "___Servis Terakhir___\n";
        cout << "Mobil: " << servisTerakhir->model << endl;
        cout << "Kendala: " << servisTerakhir->kendala << endl;
        cout << "Montir: " << servisTerakhir->montir << endl;
    }
    else{
        cout << "Belum pernah melakukan servis.\n";
    }

    cout << "-----------------------\n";

    cari = cari->next;

} while(cari != headCust);

system("pause");
}

void servisSelesai(){

    if(frontServis == NULL){
        cout << "Tidak ada servis di antrian\n";
        system("pause");
        return;
    }

    string daftarMontir[100];
    int jumlahMontir = 0;


    servis* cari = frontServis;

    while(cari != NULL){

        bool montirAda = false;

        for(int i=0;i<jumlahMontir;i++){
            if(toLowerCase(daftarMontir[i]) == toLowerCase(cari->montir)){
                montirAda = true;
                break;
            }
        }

        if(!montirAda){
            daftarMontir[jumlahMontir] = cari->montir;
            jumlahMontir++;
        }

        cari = cari->next;
    }

    cout << "\nPilih Montir!\n";

    for(int i=0;i<jumlahMontir;i++){
        cout << i+1 << ". " << daftarMontir[i] << endl;
    }

    int pilih;
    cout << "Pilihan: ";
    cin >> pilih;

    if(pilih < 1 || pilih > jumlahMontir){
        cout << "Pilihan tidak valid\n";
        system("pause");
        return;
    }

    string montir = daftarMontir[pilih-1];

    servis* prev = NULL;
    cari = frontServis;
    servis* target = NULL;
    servis* prevTarget = NULL;

    while(cari != NULL){

        if(toLowerCase(cari->montir) == toLowerCase(montir)){
            target = cari;
            prevTarget = prev;
            break;
        }

        prev = cari;
        cari = cari->next;
    }

    if(target == NULL){
        cout << "Tidak ada servis untuk montir tersebut\n";
        system("pause");
        return;
    }

    cout << "\n======== SERVIS ========\n";
    cout << "Tanggal Masuk :" << target->masuk << endl;
    cout << "Model Mobil :" << target->model << endl;
    cout << "Merek Mobil :" << target->merek << endl;
    cout << "Kendala :" << target->kendala << endl;
    cout << "Montir :" << target->montir << endl;
    cout << "Nama Pelanggan :" << target->namac << endl;
    cout << "Telp Pelanggan :" << target->noc << endl;

    char status;

    cout << "\nApakah servis ini sudah selesai? (Y/N): ";
    cin >> status;

    status = tolower(status);

    if(status == 'y'){

        if(prevTarget == NULL){
            frontServis = target->next;
        }
        else{
            prevTarget->next = target->next;
        }

        if(target == rearServis){
            rearServis = prevTarget;
        }

        target->next = selesaiServis;
        selesaiServis = target;

        simpanHistory(target);

        cout << "Servis sudah dinyatakan selesai\n";
    }
    else if(status == 'n'){

        cout << "Servis tetap berada di antrian\n";
    }
    else{

        cout << "Input tidak valid\n";
    }

    simpanServis();

    system("pause");
}

void riwayatServis (){
    cin.ignore();
    string nama;

    cout << "masukkan nama pelanggan: ";
    getline (cin,nama);

    servis* bantu = selesaiServis;

    bool ada = false;

    cout << "\n------------- Riwayat Servis -------------\n";
    cout << "SUDAH SELESAI DIKERJAKAN : \n";

    while (bantu != NULL){
        if(toLowerCase(bantu->namac) == toLowerCase(nama)){
            
            ada = true;
            cout << "\n========== SUDAH SELESAI ==========\n";
            cout << "tanggal masuk :" << bantu -> masuk << endl;
            cout << "Model mobil :" << bantu -> model << endl;
            cout << "Merek mobil :" << bantu -> merek << endl;
            cout << "Kendala :" << bantu -> kendala << endl;
            cout << "Montir :" << bantu -> montir << endl;
            cout << "No telp :" << bantu -> noc << endl;
            cout << "===================================\n";
        }
        bantu = bantu-> next;
    }
    if(!ada){
        cout <<"\nbelum ada servis yang sudah selesai \n" ;
    }

    cout << "---------------------------------------------";
    servis* cari = frontServis;
    bool antrian = false ;

    cout << "\nSEDANG DALAM PROSES PENGERJAAN : \n";
    
    while (cari != NULL){
        if(toLowerCase(cari->namac) == toLowerCase(nama)){

            antrian = true ;
            cout << "\n========== DALAM PROSES ==========\n";
            cout << "tanggal masuk :" << cari -> masuk << endl;
            cout << "Model mobil :" << cari -> model << endl;
            cout << "Merek mobil :" << cari -> merek << endl;
            cout << "Kendala :" << cari -> kendala << endl;
            cout << "Montir :" << cari -> montir << endl;
            cout << "No telp :" << cari -> noc << endl;
            cout << "===================================\n";
        }
        cari = cari-> next ;
    }
    if(!antrian){
        cout << "tidak ada servis yang sedang di kerjakan\n";
    }
system ("pause");
}

void montir() {

    if(headMontir == NULL){
        cout << "Belum ada montir terdaftar\n";
        system("pause");
        return;
    }

    cout << "\n====== RIWAYAT MONTIR ======\n";
    cout << "Pilih Montir:\n";
    tampilMontir();

    int pilih;
    cout << "Pilihan: ";
    cin >> pilih;

    Datamontir* scanner = headMontir;
    int nomor = 1;
    string namaMontir = "";

    while(scanner != NULL){
        if(nomor == pilih){
            namaMontir = scanner->nama;
            break;
        }
        scanner = scanner->next;
        nomor++;
    }

    if(namaMontir == ""){
        cout << "Pilihan tidak valid!\n";
        system("pause");
        return;
    }

    string inputLower = toLowerCase(namaMontir);

    cout << "\n====== Riwayat Kerja Montir: " << namaMontir << " ======\n";

    servis* bantu = frontServis;
    bool adaAntrian = false;

    cout << "\n--- Servis yang Sedang Dikerjakan ---\n";

    while(bantu != NULL){

        if(toLowerCase(bantu->montir) == inputLower){

            adaAntrian = true;

            cout << "-----------------------------\n";
            cout << "Tanggal Masuk : " << bantu->masuk << endl;
            cout << "Model Mobil   : " << bantu->model << endl;
            cout << "Merek Mobil   : " << bantu->merek << endl;
            cout << "Kendala       : " << bantu->kendala << endl;
            cout << "Pelanggan     : " << bantu->namac << endl;
            cout << "No Telp       : " << bantu->noc << endl;
        }

        bantu = bantu->next;
    }

    if(!adaAntrian){
        cout << "Tidak ada servis yang sedang dikerjakan\n";
    }

    // SERVIS YANG SUDAH SELESAI
    servis* history = selesaiServis;
    bool adaHistory = false;

    cout << "\n--- Servis Yang Selesai ---\n";

    while(history != NULL){

        if(toLowerCase(history->montir) == inputLower){

            adaHistory = true;

            cout << "-----------------------------\n";
            cout << "Tanggal Servis: " << history->masuk << endl;
            cout << "Model Mobil   : " << history->model << endl;
            cout << "Merek Mobil   : " << history->merek << endl;
            cout << "Kendala       : " << history->kendala << endl;
            cout << "Pelanggan     : " << history->namac << endl;
            cout << "No Telp       : " << history->noc << endl;
        }

        history = history->next;
    }

    if(!adaHistory){
        cout << "Belum ada pekerjaan yang selesai\n";
    }

    system("pause");
}

// FILE HANDLING

void simpanCustomer(){
    if(headCust == NULL) 
    return;

    ofstream file("Customer.txt");
    cust* bantu = headCust;

    do{
        file << bantu->nama << "|"
            << bantu->umur << "|"
            << bantu->gender << "|"
            << bantu->no << "|"
            << bantu->alamat << endl;

        bantu = bantu->next;

    } while(bantu != headCust);

    file.close();
}

void simpanServis(){
    ofstream file("Antrian.txt");
    servis* bantu = frontServis;

    while(bantu != NULL){
        file<< bantu->masuk << "|"  
            << bantu->model << "|"
            << bantu->merek << "|"
            << bantu->kendala << "|"
            << bantu->montir << "|"
            << bantu->namac << "|"
            << bantu->noc << endl;

        bantu = bantu->next;
    }
    file.close();
}

void simpanHistory(servis* simpan){

    ofstream file ("HistoryServis.txt", ios::app);

    file    << simpan->masuk << "|"
            << simpan->model << "|"
            << simpan->merek << "|"
            << simpan->kendala << "|"
            << simpan->montir << "|"
            << simpan->namac << "|"
            << simpan->noc << endl;

    file.close();
}

void loadCustomer(){
    ifstream file("Customer.txt");
    if(!file.is_open())
    return;

    string line;

    while(getline(file, line)){
        stringstream ss(line);
        string nama, umurStr, gender, noStr, alamat;

        getline(ss, nama, '|');
        getline(ss, umurStr, '|');
        getline(ss, gender, '|');
        getline(ss, noStr, '|');
        getline(ss, alamat, '|');

        tambahCust(nama, stoi(umurStr), gender, stoll(noStr), alamat);
    }

    file.close();
}

void loadAntrian() {

    ifstream file("Antrian.txt");

    if(!file.is_open())
    return;

    string line;

    while (getline(file, line)) {

        stringstream ss(line);
        string masuk, model, merek, kendala, montir, namac, noStr;

        getline(ss, masuk, '|');
        getline(ss, model, '|');
        getline(ss, merek, '|');
        getline(ss, kendala, '|');
        getline(ss, montir, '|');
        getline(ss, namac, '|');
        getline(ss, noStr, '|');

        tambahServisFile(masuk, model, merek, kendala, montir, namac, stoll(noStr));
    }

    file.close();
}

void loadHistory(){
    ifstream file ("HistoryServis.txt");

    if(!file.is_open())
        return;
    
        string line;

        while(getline(file,line)){

        stringstream ss(line);

        string masuk, model, merek, kendala, montir, namac, noStr;

        getline(ss, masuk, '|');
        getline(ss, model, '|');
        getline(ss, merek, '|');
        getline(ss, kendala, '|');
        getline(ss, montir, '|');
        getline(ss, namac, '|');
        getline(ss, noStr, '|');

        servis* baru = new servis;

        baru->masuk = masuk;
        baru->model = model;
        baru->merek = merek;
        baru->kendala = kendala;
        baru->montir = montir;
        baru->namac = namac;
        baru->noc = stoll(noStr);

        baru->next = selesaiServis;
        selesaiServis = baru;
        }
    file.close();
}

// MENU

void menuCustomer(string nama,long long no){

    int pilih;

    do{

        cout << "\n====== Welcome " << nama << " ======\n";
        cout << "1. Antrian Anda\n";
        cout << "2. Booking Servis\n";
        cout << "3. Batalkan Servis\n";
        cout << "4. Undo Pembatalan\n";
        cout << "5. Riwayat Servis\n";
        cout << "6. Keluar\n";
        cout << "Pilihan: ";
        cin >> pilih;

        switch(pilih){

            case 1:
                antrianCustomer(nama);
                break;

            case 2:
                bookingServisCustomer(nama,no);
                break;

            case 3:
                batalkanServis(nama,no);
                break;

            case 4:
                undoPembatalan();
                break;

            case 5:
                riwayatServis();
                break;
        }

    }while(pilih != 6);
}

void menuServis() {
    int pilih;

    do {
        cout << "\n====== Services ======\n";
        cout << "1. Semua Servis Singkat\n";
        cout << "2. Servis Baru\n";
        cout << "3. Selesaikan Pekerjaan\n";
        cout << "4. Riwayat Kerja Montir\n";
        cout << "5. Kembali\n";
        cout << "Pilihan: ";
        cin >> pilih;

        switch(pilih) {
            case 1:
                dataSS(); 
                break;
            case 2:
                tambahSS(); 
                break;
            case 3:
                servisSelesai();
                break;
            case 4:
                montir();
                break;
        }

    } while(pilih != 5);
}

void menuAdmin() {

    int pilih ;

    do{

        cout << "\n====== Welcome to Lognuts ======\n";
        cout << "Pilih menu!\n";
        cout << "1. Servis\n";
        cout << "2. Pelanggan Baru\n";
        cout << "3. Montir Baru\n";
        cout << "4. Data pelanggan\n";
        cout << "5. Semua Data Customer\n";
        cout << "6. Keluar\n";
        cout << "Pilihan :" ;
        cin >> pilih;

        switch (pilih){

            case 1 :
                menuServis();
                break;

            case 2 : {

                cin.ignore();
                string nama, gender, alamat;
                long long no ;
                int umur;

                cout << "Nama : ";
                getline (cin, nama);

                cout << "No HP : ";
                cin >> no ;
                cin.ignore();

                cout << "Alamat : ";
                getline (cin, alamat);

                cout << "Umur : ";
                cin >>  umur;
                cin.ignore();

                cout << "gender : ";
                getline (cin, gender);

                tambahCust(nama,umur,gender,no,alamat);
                simpanCustomer();

                cout << "Pelanggan berhasil didaftarkan\n" ;
                system("pause");
                break;
            }

            case 3 :
                tambahMontir();
                break;
            
            case 4:
                dataCust();
                break ;

            case 5 :
                semuaDataCust();
                break ;


        }

    }
    while (pilih !=6);
}

// PROGRAM UTAMA

int main() {

    loadCustomer();
    loadAntrian();
    loadHistory();
    loadMontir();
    loadStack();

    string namaLogin;
    long long noLogin;

    cout << "====== Welcome To Garasi Suby ======" << endl;
    cout << "\nMasukkan nama & Nomor Telepon\n" << endl;

    cout << "Nama: ";
    getline(cin, namaLogin);

    if(namaLogin == "adminacces8008"){
        menuAdmin();
        return 0;
    }

    cout << "No Telp: ";
    cin >> noLogin;

    menuCustomer(namaLogin, noLogin);

}
