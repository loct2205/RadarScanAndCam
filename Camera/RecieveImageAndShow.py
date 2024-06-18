import serial
import serial.tools.list_ports
import cv2

def liet_ke_cac_cong_serial():
    ports = serial.tools.list_ports.comports()
    danh_sach_cong = [str(port) for port in ports]
    if not danh_sach_cong:
        print("Không tìm thấy cổng serial nào.")
        return []
    for index, port in enumerate(danh_sach_cong):
        print(f"{index}: {port}")
    return danh_sach_cong

def chon_cong_serial(danh_sach_cong):
    while True:
        try:
            lua_chon = int(input("Chọn cổng bằng chỉ số: "))
            if 0 <= lua_chon < len(danh_sach_cong):
                return danh_sach_cong[lua_chon].split()[0]  # Lấy tên cổng
            else:
                print("Lựa chọn không hợp lệ. Vui lòng chọn chỉ số hợp lệ.")
        except ValueError:
            print("Đầu vào không hợp lệ. Vui lòng nhập một số.")

def doc_du_lieu_tu_esp32(serial_inst):
    while True:
        start_byte = serial_inst.read(1)
        if start_byte == b'<':
            print("Nhận kích thước ảnh...")
            size_bytes = serial_inst.read(4)
            size = int.from_bytes(size_bytes, byteorder='little')
            if size <= 0 or size > 10 * 1024 * 1024:
                print(f"Kích thước ảnh không hợp lý: {size} bytes")
                return None
            print(f"Kích thước ảnh: {size} bytes")
            image_data = bytearray()
            print("Nhận dữ liệu ảnh...")
            while len(image_data) < size:
                remaining = size - len(image_data)
                chunk_size = min(remaining, 512)
                image_data.extend(serial_inst.read(chunk_size))
            end_byte = serial_inst.read(1)
            if end_byte == b'>':
                print("Nhận dữ liệu ảnh hoàn thành.")
                return image_data
            else:
                print("Ký tự kết thúc không đúng.")
                return None

def main():
    danh_sach_cong = liet_ke_cac_cong_serial()
    if not danh_sach_cong:
        return
    ten_cong = chon_cong_serial(danh_sach_cong)
    serial_inst = serial.Serial()
    serial_inst.baudrate = 115200
    serial_inst.port = ten_cong
    try:
        serial_inst.open()
    except serial.SerialException as e:
        print(f"Không thể mở cổng {ten_cong}: {e}")
        return
    print(f"Đang đọc từ {ten_cong}. Nhấn Ctrl+C để thoát.")
    try:
        while True:
            if serial_inst.in_waiting:
                image_data = doc_du_lieu_tu_esp32(serial_inst)
                if image_data:
                    with open('image.jpg', 'wb') as image_file:
                        image_file.write(image_data)
                    print("Ảnh đã được lưu thành 'image.jpg'.")
                    image = cv2.imread('image.jpg')
                    if image is not None:
                        cv2.imshow('Received Image', image)
                        cv2.waitKey(0)
                        cv2.destroyAllWindows()
                    else:
                        print("Không thể đọc ảnh.")
    except KeyboardInterrupt:
        print("\nĐang thoát.")
    finally:
        serial_inst.close()

if __name__ == "__main__":
    main()