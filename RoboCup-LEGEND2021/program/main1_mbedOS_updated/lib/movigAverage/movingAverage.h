#ifndef _AVERAGE_H_
#define _AVERAGE_H_

// 移動平均演算器
template <typename TYPE>
class Average
{
public:
TYPE value;         // 平均値

private:
TYPE *buff;         // リングバッファ
int size;           // リングバッファのサイズ
int index;          // リングバッファのインデックス
TYPE accum;         // 合計値
bool isFirst;       // 最初のサンプルか？

public:
// コンストラクタ
// buff: リングバッファ
// size: リングバッファのサイズ
Average(TYPE* buff, int size)
{
        this->buff = buff;
        this->size = size;
        this->isFirst = true;
}

// 初期化する
void init()
{
        isFirst = true;
        value = 0;
}

// 平均値を計算する
// input_val: 入力データ
// return: 平均値
TYPE calc(TYPE input_val)
{
        if (isFirst)
        {
                // 最初のデータのときリングバッファと合計値を初期化する
                accum = 0;
                for (int i = 0; i < size; i++) {
                        buff[i] = input_val;
                        accum += (float)input_val;
                }
                value = input_val;
                index = 0;
                isFirst = false;
        }
        else
        {
                // いちばん古いデータを捨てて新しいデータを入れる
                accum -= buff[index];
                accum += input_val;
                buff[index] = input_val;
                index++;
                if (index >= size) index = 0;
                // 合計値を総数で割ると平均値
                value = accum / size;
        }
        return value;
}
};

// Averageのインスタンスを生成するマクロ
#define Average_create(TYPE, size, name) \
        TYPE _ ## name ## _buff[size]; \
        Average<TYPE> name(_ ## name ## _buff, size)

#endif
