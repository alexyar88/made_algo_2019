//
// Created by user on 28.11.19.
//

#ifndef INC_9_HUFFMAN_H
#define INC_9_HUFFMAN_H
typedef unsigned char byte;

struct IInputStream {
    IInputStream() {};
    IInputStream(std::string buffer_) {
        buffer = buffer_;
    }
    bool Read(byte& value);
    std::string buffer = "Mama mila ramy";
    int position = 0;

};

bool IInputStream::Read(byte &value) {
    if (position < buffer.size()) {
        value = buffer[position];
        ++position;
        return true;
    } else {
        return false;
    }

}


struct IOutputStream {
    void Write(byte value);
    std::string buffer;
};

void IOutputStream::Write(byte value) {
    std::cout << value;
    buffer += value;
}


#endif //INC_9_HUFFMAN_H
