#pragma once
#include <cstddef>

// std::vectorなどでアラインされた領域を用いる - koturnの日記 https://koturn.hatenablog.com/entry/2016/08/12/010000
#if defined(_MSC_VER) || defined(__MINGW32__)
#  include <malloc.h>
#endif  // defined(_MSC_VER) || defined(__MINGW32__)


/*!
 * @brief アラインメントされたメモリを動的確保する関数
 * @tparam T  確保するメモリの要素型．この関数の返却値はT*
 * @param [in] nBytes     確保するメモリサイズ (単位はbyte)
 * @param [in] alignment  アラインメント (2のべき乗を指定すること)
 * @return  アラインメントし，動的確保されたメモリ領域へのポインタ
 */
template<typename T = void>
static inline T*
alignedMalloc(std::size_t nBytes, std::size_t alignment = alignof(T)) noexcept
{
#if defined(_MSC_VER) || defined(__MINGW32__)
  return reinterpret_cast<T*>(::_aligned_malloc(nBytes, alignment));
#else
  void* p;
  return reinterpret_cast<T*>(::posix_memalign(&p, alignment, nBytes) == 0 ? p : nullptr);
#endif  // defined(_MSC_VER) || defined(__MINGW32__)
}


/*!
 * @brief アラインメントされたメモリを動的確保する関数．配列向けにalignedMallocの引数指定が簡略化されている
 * @tparam T  確保する配列の要素型．この関数の返却値はT*
 * @param [in] size       確保する要素数．すなわち確保するサイズは size * sizeof(T)
 * @param [in] alignment  アラインメント (2のべき乗を指定すること)
 * @return  アラインメントし，動的確保されたメモリ領域へのポインタ
 */
template<typename T>
static inline T*
alignedAllocArray(std::size_t size, std::size_t alignment = alignof(T)) noexcept
{
  return alignedMalloc<T>(size * sizeof(T), alignment);
}


/*!
 * @brief アラインメントされたメモリを解放する関数
 * @param [in] ptr  解放対象のメモリの先頭番地を指すポインタ
 */
static inline void
alignedFree(void* ptr) noexcept
{
#if defined(_MSC_VER) || defined(__MINGW32__)
  ::_aligned_free(ptr);
#else
  std::free(ptr);
#endif  // defined(_MSC_VER) || defined(__MINGW32__)
}


/*！
 * @brief アラインされたメモリ領域を確保するアロケータ
 */
template<
  typename T,
  std::size_t kAlignment = alignof(T)
>
class AlignedAllocator
{
public:
  //! 要素型
  using value_type = T;
  //! サイズ型
  using size_type = std::size_t;
  //! 要素のポインタ型
  using pointer = typename std::add_pointer<value_type>::type;
  //! 要素の読み取り専用ポインタ型
  using const_pointer = typename std::add_pointer<const value_type>::type;

  /*!
   * @brief 再束縛のための別の要素型のアロケータの定義
   *
   * AlignedAllocatorが2つのテンプレート引数を持つため，この定義は必須
   */
  template<class U>
  struct rebind
  {
    //! 再束縛のためのアロケータ型
    using other = AlignedAllocator<U, kAlignment>;
  };

  /*!
   * デフォルトコンストラクタ
   */
  AlignedAllocator() noexcept
  {}

  /*!
   * 別の要素型を受け取るコンストラクタ
   */
  template<typename U>
  AlignedAllocator(const AlignedAllocator<U, kAlignment>&) noexcept
  {}

  /*!
   * @brief メモリ領域を動的確保する
   *
   * @param [in] n     確保する要素数
   * @param [in] hint  ヒント（使用しない）
   * @return  アラインされたメモリ領域へのポインタ
   */
  pointer
    allocate(size_type n, const_pointer /* hint */ = nullptr) const
  {
    auto p = alignedAllocArray<value_type>(n, kAlignment);
    if (p == nullptr) {
      throw std::bad_alloc{};
    }
    return p;
  }

  /*!
   * @brief メモリ領域を解放する
   * @param [in,out] p  動的確保したメモリ領域
   * @param [in]     n  要素数 (使用しない引数)
   */
  void
    deallocate(pointer p, size_type /* n */) const noexcept
  {
    alignedFree(p);
  }
};  // class AlignedAllocator


template<
  typename T,
  std::size_t kAlignment1,
  typename U,
  std::size_t kAlignment2
>
static inline bool
operator==(const AlignedAllocator<T, kAlignment1>&, const AlignedAllocator<U, kAlignment2>&) noexcept
{
  return kAlignment1 == kAlignment2;
}


template<
  typename T,
  std::size_t kAlignment1,
  typename U,
  std::size_t kAlignment2
>
static inline bool
operator!=(const AlignedAllocator<T, kAlignment1>& lhs, const AlignedAllocator<U, kAlignment2>& rhs) noexcept
{
  return !(lhs == rhs);
}
