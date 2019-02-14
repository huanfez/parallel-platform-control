/******************************************************************************/
/*                                                                            */
/*                  レプトリノ製 力覚センサ通信ライブラリ                     */
/*                                                                            */
/******************************************************************************/
extern "C" __declspec(dllexport) void __stdcall Initialize();								// ＤＬＬの初期化処理
extern "C" __declspec(dllexport) void __stdcall Finalize();									// ＤＬＬの終了処理
extern "C" __declspec(dllexport) bool __stdcall PortOpen(int pPortNo);						// ポートオープン
extern "C" __declspec(dllexport) void __stdcall PortClose(int pPortNo);						// ポートクローズ
extern "C" __declspec(dllexport) bool __stdcall SetSerialMode(int pPortNo,bool pSet);		// データの連続読込の開始/停止
extern "C" __declspec(dllexport) bool __stdcall GetSerialData(int pPortNo,double *pData,char *pStatus);	// 連続データ読込み
extern "C" __declspec(dllexport) bool __stdcall GetLatestData(int pPortNo,double *pData,char *pStatus);	// 最新データ読込
extern "C" __declspec(dllexport) bool __stdcall GetSensorLimit(int pPortNo,double *pData);	// センサ定格確認
extern "C" __declspec(dllexport) bool __stdcall GetSensorInfo(int pPortNo,char *psSerialNo); // センサシリアルNo取得

