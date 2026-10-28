// 編譯：g++ -std=c++26 -freflection C-metafunctions/c6_book_json_demo.cpp -o /tmp/book_json_demo
// 學習用獨立副本。共用函式改編自團隊 c4_to_json.cpp，原始檔未修改。
// 示範新增 Book 與呼叫；AI 協助整理，GCC 16.2 驗證紀錄另存。
#include <meta>
#include <iostream>
#include <string>


// 基本型別：一般的函式多載
void write_json(std::ostream& out, int v) { out << v; }
void write_json(std::ostream& out, double v) { out << v; }
void write_json(std::ostream& out, const std::string& v) { out << '"' << v << '"'; }   // 未處理跳脫字元

// 其他型別：當成 struct，用反射逐一處理成員
template <typename T>
void write_json(std::ostream& out, const T& obj) {
    static_assert(std::meta::is_class_type(^^T), "write_json only supports int, double, std::string and structs");

    out << "{";
    bool first = true;
    template for (constexpr std::meta::info m :
                  std::define_static_array(std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::current()))) {
        if (!first)
            out << ", ";
        out << '"' << std::meta::identifier_of(m) << "\": ";
        write_json(out, obj.[: m :]);    // 成員如果也是 struct，就會再走進這個函式
        first = false;
    }
    out << "}";
}


struct Book {
    std::string title;
    int pages;
    double price;
};

int main() {
    write_json(std::cout, Book{"CPP", 320, 499.5});
    std::cout << "\n";
}

// 預期輸出：
// {"title": "CPP", "pages": 320, "price": 499.5}
