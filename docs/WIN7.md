# BLACK di Windows 7 32-bit (RAM 2 GB)

Perubahan di kode: tidak lagi bergantung pada fungsi Windows 8/10 (`GetSystemTimePreciseAsFileTime`, `BCryptHash`),
`d3dcompiler` dimuat saat jalan (tidak di-link), suara jatuh ke `waveOut` jika XAudio2 8/9 tidak ada, pengaturan
awal ringan (scale 1, tanpa AA, 30 fps) jika RAM <= 3 GB, dan pesan error jelas jika alamat 0x80000000 tidak tersedia.

## Yang harus disiapkan di PC Win 7
1. Windows 7 **SP1**.
2. **DirectX End-User Runtime (June 2010)** (memberi d3dcompiler_43.dll) -- atau salin d3dcompiler_47.dll 32-bit ke sebelah black.exe.
3. **Wajib**: game butuh 64 MB di alamat 0x80000000, padahal Windows 32-bit hanya memberi 2 GB. Buka CMD sebagai administrator:
       bcdedit /set IncreaseUserVa 3072
   lalu restart. (Batalkan: `bcdedit /deletevalue IncreaseUserVa`.)
4. Kartu grafis dengan Direct3D 9 + driver terbaru.

## Build (butuh Windows 10/11 atau GitHub Actions; VS 2022 tidak jalan di Win 7)
Pasang komponen "MSVC v142 - VS 2019 C++ x64/x86 build tools" di VS 2022, lalu:
    cmake --preset win32-win7
    cmake --build build/win32-win7 --config RelWithDebInfo
Atau jalankan workflow build-win7 di tab Actions dan ambil artifact `black-win7`.
Belum dicoba dikompilasi/dijalankan di Win 7 -- laporkan `black.log` jika ada masalah.
