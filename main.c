// 1. Phân tích lỗi & Bảng Test Cases (Dành cho tệp README.md)Nguyên nhân kỹ thuật:Lỗi đồng bộ dữ liệu: Biến soLuong (kiểu int) khi được truyền vào hàm datVeConcert trong C mặc định là truyền giá trị (copy giá trị). Dòng soLuong++; chỉ thay đổi bản sao cục bộ. Khi quay lại main(), biến soLuong gốc vẫn giữ nguyên giá trị 0.Lỗi thiếu logic nghiệp vụ: Hàm datVeConcert chỉ kiểm tra tổng sức chứa mảng MAX_VE (100 vé) mà chưa thiết lập chốt chặn quy định MAX_VE_PER_ORDER (4 vé/đơn).Bảng 02 Test Cases Đối chứng:Trường hợp kiểm thửDữ liệu đầu vàoKết quả sai thực tế (Code cũ)Kết quả đúng mong đợi (Code mới)TC01: Đồng bộ số lượng véKhách hàng thực hiện đặt thành công 2 vé hợp lệ đầu tiên.Hiển thị: "[THÔNG BÁO]: Hiện chưa có vé nào được đặt thành công" (soLuong = 0).Cập nhật chính xác soLuong = 2 trong main(), hiển thị chi tiết 2 vé trong giỏ hàng.TC02: Giới hạn hạn ngạchKhách hàng cố tình gửi yêu cầu đặt liên tiếp 5 vé trong một đơn.Hệ thống thông báo thêm thành công cả 5 vé, vi phạm quy định chống đầu cơ.4 vé đầu thành công. Vé thứ 5 bị từ chối với cảnh báo "[LỖI]: Hạn ngạch vượt qua quy định!".

#include <stdio.h>
#include <string.h>

#define MAX_VE 100
#define MAX_VE_PER_ORDER 4

// Cấu trúc lưu trữ thông tin vé Concert
struct VeBan {
    int maVe;
    char tenKhuVuc[20];
    float giaGoc;
    int isEarlyBird; // 1: Được giảm 15%, 0: Giá gốc
};

// SỬA LỖI 1: Thay đổi kiểu trả về thành int để cập nhật soLuong về main()
int datVeConcert(struct VeBan dsVe[], int soLuong, struct VeBan veMoi);
void hienThiDanhSachVe(const struct VeBan dsVe[], int soLuong);

int main() {
    struct VeBan dsVe[MAX_VE];
    int soLuong = 0;

    printf("=== HE THONG DAT VE CONCERT TICKETBOX ===\n\n");

    // Giả lập khách hàng đặt 5 vé (kiểm tra chặn giới hạn 4 vé)
    struct VeBan ve1 = {101, "VIP", 2000000.0f, 1};
    struct VeBan ve2 = {102, "Zone A", 1200000.0f, 0};
    struct VeBan ve3 = {103, "Zone A", 1200000.0f, 0};
    struct VeBan ve4 = {104, "GA", 500000.0f, 1};
    struct VeBan ve5 = {105, "GA", 500000.0f, 0}; // Vé thứ 5 sẽ bị chặn

    // SỬA LỖI 1: Gán lại giá trị soLuong sau mỗi lần gọi hàm
    soLuong = datVeConcert(dsVe, soLuong, ve1);
    soLuong = datVeConcert(dsVe, soLuong, ve2);
    soLuong = datVeConcert(dsVe, soLuong, ve3);
    soLuong = datVeConcert(dsVe, soLuong, ve4);
    soLuong = datVeConcert(dsVe, soLuong, ve5); 

    // Hiển thị lại giỏ hàng sau khi đặt vé
    printf("\n>>> KET QUA KIEM TRA GIO HANG TRONG MAIN <<<\n");
    hienThiDanhSachVe(dsVe, soLuong);

    return 0;
}

int datVeConcert(struct VeBan dsVe[], int soLuong, struct VeBan veMoi) {
    if (soLuong >= MAX_VE) {
        printf("[LOI]: Mang luu tru ve cua he thong da day!\n");
        return soLuong; // Vẫn trả về số lượng hiện hành
    }

    // SỬA LỖI 2: Thêm logic kiểm tra hạn ngạch mua vé (Tối đa 4 vé/đơn)
    if (soLuong >= MAX_VE_PER_ORDER) {
        printf("[LOI]: Han ngach vuot qua quy dinh! Chi duoc dat toi da %d ve cho moi giao dich (Ve %d bi tu choi).\n", MAX_VE_PER_ORDER, veMoi.maVe);
        return soLuong;
    }

    // Tính giá thực tế sau khi áp dụng Early Bird (giảm 15%)
    if (veMoi.isEarlyBird == 1) {
        veMoi.giaGoc = veMoi.giaGoc * 0.85f;
    }

    dsVe[soLuong] = veMoi;
    soLuong++; 
    
    printf("[THANH CONG]: Da them ve %d (%s) vao don hang.\n", veMoi.maVe, veMoi.tenKhuVuc);
    
    // SỬA LỖI 1: Trả về số lượng mới để đồng bộ ngược ra hàm main()
    return soLuong; 
}

void hienThiDanhSachVe(const struct VeBan dsVe[], int soLuong) {
    if (soLuong == 0) {
        printf("[THONG BAO]: Hien chua co ve nao duoc dat thanh cong trong he thong!\n");
        return;
    }

    printf("--- DANH SACH VE DA DAT (%d ve) ---\n", soLuong);
    for (int i = 0; i < soLuong; i++) {
        printf("STT: %d | Ma ve: %d | Khu vuc: %-7s | Gia thanh toan: %.0f VND\n",
               i + 1, dsVe[i].maVe, dsVe[i].tenKhuVuc, dsVe[i].giaGoc);
    }
}