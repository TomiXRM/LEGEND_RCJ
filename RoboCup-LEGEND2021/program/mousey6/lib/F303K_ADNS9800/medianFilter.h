#ifndef _MEDIAN_H_
#define _MEDIAN_H_

// 移動中央値演算器 (省メモリ版)
template <typename TYPE>
class Median
{
public:
TYPE value;         // 中央値

private:
TYPE *buff;         // リングバッファ
int size;           // リングバッファのサイズ
int index;          // リングバッファのインデックス
TYPE m1;            // 中央値(小) (偶数個の場合に使用)
TYPE m2;            // 中央値(大) (偶数個の場合に使用)
bool isFirst;       // 最初のサンプルか？

public:
// コンストラクタ
// buff: リングバッファ
// size: リングバッファのサイズ
Median(TYPE *buff, int size)
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

// 中央値を計算する
// input_val: 入力データ
// return: 中央値
TYPE calc(TYPE input_val)
{
        if (isFirst)
        {
                // 最初のデータのときリングバッファと中央値を初期化する
                for (int i = 0; i < size; i++) {
                        buff[i] = input_val;
                }
                value = input_val;
                m1 = m2 = input_val;
                index = 0;
                isFirst = false;
        }
        else
        {
                // いちばん古いデータを捨てて新しいデータを入れる
                TYPE old_val = buff[index];
                buff[index] = input_val;

                // 奇数個の場合
                if ((size & 1) != 0) {
                        // 現在の中央値より大きい値が入り、現在の中央値以下の値が出たとき
                        if ((input_val > value) && (old_val <= value)) {
                                // 現在の中央値の次に小さいのが次の中央値
                                int hcnt, ecnt;
                                TYPE n = next_min(value, &hcnt, &ecnt);
                                // ただし現在の中央値より大きい値が過半数でないなら現在の中央値はダブっているので更新しない
                                if (hcnt > (size / 2)) {
                                        value = n;
                                }
                        }
                        // 現在の中央値より小さい値が入り、現在の中央値以上の値が出たとき
                        else if ((input_val < value) && (old_val >= value)) {
                                // 現在の中央値の次に大きいのが次の中央値
                                int lcnt, ecnt;
                                TYPE n = next_max(value, &lcnt, &ecnt);
                                // ただし現在の中央値より小さい値が過半数でないなら現在の中央値はダブっているので更新しない
                                if (lcnt > (size / 2)) {
                                        value = n;
                                }
                        }
                }
                // 偶数個の場合
                else {
                        // 現在の中央値(大)より大きい値が入り、現在の中央値(大)以下の値が出たとき
                        if ((input_val > m2) && (old_val <= m2)) {
                                int hcnt, ecnt;
                                TYPE n = next_min(m2, &hcnt, &ecnt);
                                if (hcnt == (size / 2)) {
                                        if (m2 != old_val) {
                                                m1 = m2;
                                        }
                                        m2 = n;
                                        value = (m2 + m1) / 2;
                                }
                                else if ((hcnt + ecnt) > (size / 2)) {
                                        m1 = m2;
                                        value = m2;
                                }
                        }
                        // 現在の中央値(小)より小さい値が入り、現在の中央値(小)以上の値が出たとき
                        else if ((input_val < m1) && (old_val >= m1)) {
                                int lcnt, ecnt;
                                TYPE n = next_max(m1, &lcnt, &ecnt);
                                if (lcnt == (size / 2)) {
                                        if (m1 != old_val) {
                                                m2 = m1;
                                        }
                                        m1 = n;
                                        value = (m2 + m1) / 2;
                                }
                                else if ((lcnt + ecnt) > (size / 2)) {
                                        m2 = m1;
                                        value = m1;
                                }
                        }
                        // 中央値(大)を新しい値にする場合
                        else if((input_val < m2) && (old_val >= m2)) {
                                m2 = input_val;
                                value = (m2 + m1) / 2;
                        }
                        // 中央値(小)を新しい値にする場合
                        else if ((input_val > m1) && (old_val <= m1)) {
                                m1 = input_val;
                                value = (m2 + m1) / 2;
                        }
                }
                // index更新
                index++;
                if (index >= size) index = 0;
        }
        return value;
}
private:
// mの次に小さい値を探す (ダブりを考慮)
// hcnt: mより大きい値の数
// ecnt: mと等しい値の数
TYPE next_min(TYPE m, int* hcnt, int* ecnt)
{
        // mより大きい中で最小の値を探す
        *hcnt = 0;
        *ecnt = 0;
        TYPE n = 0;
        bool first = true;
        for (int i = 0; i < size; i++) {
                TYPE x = buff[i];
                if (x > m) {
                        (*hcnt)++;
                        if (first) {
                                first = false;
                                n = x;
                        }else if (x < n) {
                                n = x;
                        }
                } else if (x == m) {
                        (*ecnt)++;
                }
        }
        return n;
}

// mの次に大きい値を探す (ダブりを考慮)
// lcnt: mより小さい値の数
// ecnt: mと等しい値の数
TYPE next_max(TYPE m, int* lcnt, int* ecnt)
{
        // mより小さい中で最大の値を探す
        *lcnt = 0;
        *ecnt = 0;
        TYPE n = 0;
        bool first = true;
        for (int i = 0; i < size; i++) {
                TYPE x = buff[i];
                if (x < m) {
                        (*lcnt)++;
                        if (first) {
                                first = false;
                                n = x;
                        }
                        else if (x > n) {
                                n = x;
                        }
                } else if (x == m) {
                        (*ecnt)++;
                }
        }
        return n;
}
};

// Medianのインスタンスを生成するマクロ
#define Median_create(TYPE, size, name) \
        TYPE _ ## name ## _buff[size]; \
        Median<TYPE> name(_ ## name ## _buff, size)

#endif
