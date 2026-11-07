// nw4r::db::DbgPrintBase<char/wchar_t> (NW4R). Source: revvmp/pokemon-rumble-decomp src/nw4r/db/db_DbgPrintBase.cpp
// and include/nw4r/db/db_DbgPrintBase.h (MIT); replaces the unused dummy file of the Xenoblade copy.
#include <nw4r/db/db_DbgPrintBase.h>
#include <nw4r/ut/ut_TextWriterBase.h>
#include <revolution/GX.h>
#include <revolution/MTX.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>

namespace nw4r {
namespace db {

inline int Format(char* pBuffer, u32 size, const char* pFormat, va_list args) {
    return vsnprintf(pBuffer, size, pFormat, args);
}

inline int Format(wchar_t* pBuffer, u32 size, const wchar_t* pFormat, va_list args) {
    return vswprintf(pBuffer, size, pFormat, args);
}

template <typename T> MEMiHeapHead* DbgPrintBase<T>::sHeap;
template <typename T> DbgPrintBase<T>* DbgPrintBase<T>::sInstance;

template <typename T> DbgPrintBase<T>* DbgPrintBase<T>::GetInstance() {
    return sInstance;
}

template <typename T> void DbgPrintBase<T>::Create(void* pBuffer, u32 size, ut::Color color) {
    CreateHeap(pBuffer, size);
    sInstance = new DbgPrintBase(color, NULL);
}

template <typename T> void DbgPrintBase<T>::Create(void* pBuffer, u32 size, const ut::Font* pFont, ut::Color color) {
    CreateHeap(pBuffer, size);
    sInstance = new DbgPrintBase(color, pFont);
}

template <typename T> void DbgPrintBase<T>::SetFont(const ut::Font* pFont) {
    mFont = pFont;
}

template <typename T> const ut::Font* DbgPrintBase<T>::GetFont() const {
    return mFont;
}

template <typename T> void DbgPrintBase<T>::SetColor(GXColor color) {
    mColor = color;
}

template <typename T> ut::Color DbgPrintBase<T>::GetColor() const {
    return mColor;
}

template <typename T> void DbgPrintBase<T>::SetFontSize(f32 scale) {
    mFontSize = scale;
}

template <typename T> f32 DbgPrintBase<T>::GetFontSize() const {
    return mFontSize;
}

template <typename T> void DbgPrintBase<T>::SetVisible(bool visible) {
    mVisible = visible;
}

template <typename T> bool DbgPrintBase<T>::IsVisible() const {
    return mVisible;
}

template <typename T> void DbgPrintBase<T>::Printf(int x, int y, const T* pFormat, ...) {
    va_list args;
    va_start(args, pFormat);
    T* pBuffer;
    u32 size = (MEMGetAllocatableSizeForExpHeapEx(sHeap, 4) - sizeof(DbgPrintBase)) / 2;
    MEMSetGroupIDForExpHeap(sHeap, 0);
    pBuffer = static_cast<T*>(MEMAllocFromExpHeapEx(sHeap, size, -4));
    if (pBuffer != NULL) {
        int length = Format(pBuffer, size, pFormat, args);
        Append(x, y, 1, pBuffer, length);
        MEMFreeToExpHeap(sHeap, pBuffer);
    }
    va_end(args);
}

template <typename T> void DbgPrintBase<T>::Printf(int x, int y, int frames, const T* pFormat, ...) {
    va_list args;
    va_start(args, pFormat);
    int alignment = frames == 1 ? -4 : 4;
    u32 size = (MEMGetAllocatableSizeForExpHeapEx(sHeap, 4) - sizeof(DbgPrintBase)) / 2;
    MEMSetGroupIDForExpHeap(sHeap, 0);
    T* pBuffer = static_cast<T*>(MEMAllocFromExpHeapEx(sHeap, size, alignment));
    if (pBuffer != NULL) {
        int length = Format(pBuffer, size, pFormat, args);
        Append(x, y, frames, pBuffer, length);
        MEMFreeToExpHeap(sHeap, pBuffer);
    }
    va_end(args);
}

template <typename T> void DbgPrintBase<T>::VPrintf(int x, int y, int frames, const T* pFormat, va_list args) {
    int alignment = frames == 1 ? -4 : 4;
    u32 size = (MEMGetAllocatableSizeForExpHeapEx(sHeap, 4) - sizeof(DbgPrintBase)) / 2;
    MEMSetGroupIDForExpHeap(sHeap, 0);
    T* pBuffer = static_cast<T*>(MEMAllocFromExpHeapEx(sHeap, size, alignment));
    if (pBuffer != NULL) {
        int length = Format(pBuffer, size, pFormat, args);
        Append(x, y, frames, pBuffer, length);
        MEMFreeToExpHeap(sHeap, pBuffer);
    }
}

template <typename T> __declspec(noinline) void DbgPrintBase<T>::Append(int x, int y, int frames, const T* pText, int length) {
    u32 size = sizeof(Entry) + length;
    int alignment = frames == 1 ? 4 : -4;
    MEMSetGroupIDForExpHeap(sHeap, 0);
    Entry* pEntry = static_cast<Entry*>(MEMAllocFromExpHeapEx(sHeap, size, alignment));
    if (pEntry != NULL) {
        pEntry->x = x;
        pEntry->y = y;
        pEntry->frames = frames;
        pEntry->length = length;
        memcpy(pEntry->text, pText, length);
        ut::List_Append(&mEntries, pEntry);
    }
}

template <typename T> void DbgPrintBase<T>::PrintfCopy(int x, int y, const T* pFormat, ...) {
    va_list args;
    va_start(args, pFormat);
    T* pBuffer;
    u32 size = (MEMGetAllocatableSizeForExpHeapEx(sHeap, 4) - sizeof(DbgPrintBase)) / 2;
    MEMSetGroupIDForExpHeap(sHeap, 0);
    pBuffer = static_cast<T*>(MEMAllocFromExpHeapEx(sHeap, size, -4));
    if (pBuffer != NULL) {
        int length = Format(pBuffer, size, pFormat, args);
        Append(x, y, 1, pBuffer, length);
        MEMFreeToExpHeap(sHeap, pBuffer);
    }
    va_end(args);
}

template <typename T> void DbgPrintBase<T>::PrintfCopy(int x, int y, int frames, const T* pFormat, ...) {
    va_list args;
    va_start(args, pFormat);
    int alignment = frames == 1 ? -4 : 4;
    u32 size = (MEMGetAllocatableSizeForExpHeapEx(sHeap, 4) - sizeof(DbgPrintBase)) / 2;
    MEMSetGroupIDForExpHeap(sHeap, 0);
    T* pBuffer = static_cast<T*>(MEMAllocFromExpHeapEx(sHeap, size, alignment));
    if (pBuffer != NULL) {
        int length = Format(pBuffer, size, pFormat, args);
        Append(x, y, frames, pBuffer, length);
        MEMFreeToExpHeap(sHeap, pBuffer);
    }
    va_end(args);
}

template <typename T> void DbgPrintBase<T>::VPrintfCopy(int x, int y, int frames, const T* pFormat, va_list args) {
    VPrintf(x, y, frames, pFormat, args);
}

template <typename T> void DbgPrintBase<T>::AppendCopy(int x, int y, int frames, const T* pText, int length) {
    Append(x, y, frames, pText, length);
}

template <typename T> void DbgPrintBase<T>::Clear() {
    if (sHeap != NULL) {
        Entry* pEntry = GetFirst();
        while (pEntry != NULL) {
            Entry* pNext = GetNext(pEntry);
            Remove(pEntry);
            pEntry = pNext;
        }
    }
    ut::List_Init(&mEntries, offsetof(Entry, link));
}

template <typename T> void DbgPrintBase<T>::Draw() {
    ut::TextWriterBase<T> writer;
    if (mFont == NULL) {
        return;
    }
    writer.SetFont(*mFont);
    writer.SetTextColor(mColor);
    if (mFontSize >= 0.0f) {
        writer.SetFontSize(mFontSize);
    }
    writer.SetupGX();
    Entry* pEntry = GetFirst();
    while (pEntry != NULL) {
        Entry* pNext = GetNext(pEntry);
        if (mVisible) {
            writer.SetCursor(pEntry->x, pEntry->y);
            writer.Print(pEntry->text, pEntry->length);
        }
        if (--pEntry->frames <= 0) {
            Remove(pEntry);
        }
        pEntry = pNext;
    }
}

template <typename T> void DbgPrintBase<T>::Draw(int x, int y, int width, int height) {
    Mtx44 projection;
    Mtx model;
    f32 left = x;
    f32 top = y;
    f32 right = x + width;
    f32 bottom = y + height;
    C_MTXOrtho(projection, top, bottom, left, right, 0.0f, 1.0f);
    GXSetProjection(projection, GX_ORTHOGRAPHIC);
    PSMTXIdentity(model);
    GXLoadPosMtxImm(model, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    Draw();
}

template <typename T> DbgPrintBase<T>::DbgPrintBase(ut::Color color, const ut::Font* pFont)
    : mFont(pFont), mColor(color), mFontSize(-1.0f), mVisible(true) {
    ut::List_Init(&mEntries, offsetof(Entry, link));
}

template <typename T> DbgPrintBase<T>::~DbgPrintBase() {}

template <typename T> typename DbgPrintBase<T>::Entry* DbgPrintBase<T>::GetFirst() {
    return static_cast<Entry*>(ut::List_GetNext(&mEntries, NULL));
}

template <typename T> typename DbgPrintBase<T>::Entry* DbgPrintBase<T>::GetNext(Entry* pEntry) {
    return static_cast<Entry*>(ut::List_GetNext(&mEntries, pEntry));
}

template <typename T> void DbgPrintBase<T>::Remove(Entry* pEntry) {
    ut::List_Remove(&mEntries, pEntry);
    MEMFreeToExpHeap(sHeap, pEntry);
}

template <typename T> void DbgPrintBase<T>::CreateHeap(void* pBuffer, u32 size) {
    sHeap = MEMCreateExpHeapEx(pBuffer, size, 0);
}

template <typename T> void* DbgPrintBase<T>::DestroyHeap() {
    void* pBuffer = NULL;
    if (sHeap != NULL) {
        delete sInstance;
        pBuffer = sHeap;
        MEMDestroyExpHeap(sHeap);
        sHeap = NULL;
        sInstance = NULL;
    }
    return pBuffer;
}

template <typename T> void* DbgPrintBase<T>::operator new(size_t size) {
    MEMSetGroupIDForExpHeap(sHeap, 0);
    return MEMAllocFromExpHeapEx(sHeap, size, 4);
}

template <typename T> void DbgPrintBase<T>::operator delete(void* pBuffer) {
    MEMFreeToExpHeap(sHeap, pBuffer);
}

template class DbgPrintBase<char>;
template class DbgPrintBase<wchar_t>;

}
}
