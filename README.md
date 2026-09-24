# Phần mềm Quản lý Kho hàng — PBL2 (DUT)

Đồ án Cơ sở lập trình: C++, OOP, tự cài đặt cấu trúc dữ liệu (không dùng STL container).

## Cấu trúc
```
include/ds/        Cấu trúc dữ liệu (template, cài đặt trong .h)
include/models/    Lớp thực thể: Entity, Partner, Supplier, Customer, Product, Warehouse,
                   Batch, StockItem, ReceiptLine, Receipt(Import/Export/Transfer), Order, Date
include/services/  Nghiệp vụ: Catalog, InventoryService, OrderService, SearchService,
                   AlertService, ReportService, FileManager, Logger, WarehouseSystem (facade)
src/               Cài đặt .cpp tương ứng
tests/             Test từng phần (make test_<Ten>)
ui/                Giao diện (chỉ gọi qua WarehouseSystem)
data/              Dữ liệu file .txt (phân cách bằng |)
```

## Cấu trúc dữ liệu ↔ chức năng
| CTDL | Chức năng |
|---|---|
| HashTable | Danh mục mặt hàng, NCC, khách hàng, kho |
| DoublyLinkedList | Dòng phiếu nhập/xuất, danh sách lô |
| MinHeap | Xuất FEFO, cảnh báo cận date |
| MaxHeap | Gợi ý kho nguồn điều chuyển |
| Deque | Hàng đợi đơn chờ soạn (VIP lên đầu) |
| Trie | Gợi ý tên mặt hàng |
| BST | Lọc theo khoảng giá / tồn |
| FenwickTree | Doanh thu theo khoảng ngày |

## Build & chạy
```
make            # build
make run        # chạy chương trình
make test_Date  # chạy test
make clean
```
Windows: dùng MinGW (`mingw32-make`) hoặc WSL.

## Quy tắc
1. `ds/`, `models/`, `services/` **không dùng cin/cout** — trả về giá trị / `Result`.
2. Giao diện chỉ gọi `WarehouseSystem`.
3. Viết xong file nào → viết test file đó → test pass mới merge vào `main`.
4. Nhánh `main` luôn phải `make` được.

## Thứ tự làm
DoublyLinkedList → Deque → MinHeap/MaxHeap → HashTable → Trie → BST → Fenwick
→ Entity/Partner/Supplier/Customer/Product → Batch → StockItem → Warehouse → Receipt...
→ Catalog → FileManager → InventoryService → các service còn lại → main/ui
