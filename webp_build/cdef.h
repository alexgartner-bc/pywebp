typedef enum WebPPreset {
  WEBP_PRESET_DEFAULT = 0,
  WEBP_PRESET_PICTURE,
  WEBP_PRESET_PHOTO,
  WEBP_PRESET_DRAWING,
  WEBP_PRESET_ICON,
  WEBP_PRESET_TEXT
} WebPPreset;

typedef enum WEBP_CSP_MODE {
  MODE_RGB = 0, MODE_RGBA = 1,
  MODE_BGR = 2, MODE_BGRA = 3,
  MODE_ARGB = 4, MODE_RGBA_4444 = 5,
  MODE_RGB_565 = 6,
  MODE_rgbA = 7,
  MODE_bgrA = 8,
  MODE_Argb = 9,
  MODE_rgbA_4444 = 10,
  MODE_YUV = 11, MODE_YUVA = 12,
  MODE_LAST = 13
} WEBP_CSP_MODE;

typedef enum VP8StatusCode {
  VP8_STATUS_OK = 0,
  VP8_STATUS_OUT_OF_MEMORY,
  VP8_STATUS_INVALID_PARAM,
  VP8_STATUS_BITSTREAM_ERROR,
  VP8_STATUS_UNSUPPORTED_FEATURE,
  VP8_STATUS_SUSPENDED,
  VP8_STATUS_USER_ABORT,
  VP8_STATUS_NOT_ENOUGH_DATA
} VP8StatusCode;

struct WebPData {
  const uint8_t* bytes;
  size_t size;
  ...;
};
typedef struct WebPData WebPData;

struct WebPPicture;
typedef struct WebPPicture WebPPicture;

typedef int (*WebPWriterFunction)(const uint8_t* data, size_t data_size, const WebPPicture* picture);

struct WebPPicture {
  int use_argb;
  int width;
  int height;
  WebPWriterFunction writer;
  void* custom_ptr;
  ...;
};

struct WebPRGBABuffer {
  uint8_t* rgba;
  int stride;
  size_t size;
};
typedef struct WebPRGBABuffer WebPRGBABuffer;

struct WebPYUVABuffer {
  uint8_t* y, *u, *v, *a;
  int y_stride;
  int u_stride, v_stride;
  int a_stride;
  size_t y_size;
  size_t u_size, v_size;
  size_t a_size;
};
typedef struct WebPYUVABuffer WebPYUVABuffer;

struct WebPBitstreamFeatures {
  int width;
  int height;
  int has_alpha;
  int has_animation;
  int format;
  ...;
};
typedef struct WebPBitstreamFeatures WebPBitstreamFeatures;

struct WebPDecBuffer {
  WEBP_CSP_MODE colorspace;
  int width, height;
  int is_external_memory;
  union {
    WebPRGBABuffer RGBA;
    WebPYUVABuffer YUVA;
  } u;
  ...;
};
typedef struct WebPDecBuffer WebPDecBuffer;

struct WebPDecoderOptions {
  int use_threads;
  ...;
};
typedef struct WebPDecoderOptions WebPDecoderOptions;

struct WebPDecoderConfig {
  WebPBitstreamFeatures input;
  WebPDecBuffer output;
  WebPDecoderOptions options;
  ...;
};
typedef struct WebPDecoderConfig WebPDecoderConfig;

// Image characteristics hint for the underlying encoder.
typedef enum WebPImageHint {
  WEBP_HINT_DEFAULT = 0,  // default preset.
  WEBP_HINT_PICTURE,      // digital picture, like portrait, inner shot
  WEBP_HINT_PHOTO,        // outdoor photograph, with natural lighting
  WEBP_HINT_GRAPH,        // Discrete tone image (graph, map-tile etc).
  WEBP_HINT_LAST
} WebPImageHint;

// Compression parameters.
struct WebPConfig {
  int lossless;           // Lossless encoding (0=lossy(default), 1=lossless).
  float quality;          // between 0 and 100. For lossy, 0 gives the smallest
                          // size and 100 the largest. For lossless, this
                          // parameter is the amount of effort put into the
                          // compression: 0 is the fastest but gives larger
                          // files compared to the slowest, but best, 100.
  int method;             // quality/speed trade-off (0=fast, 6=slower-better)

  WebPImageHint image_hint;  // Hint for image type (lossless only for now).

  int target_size;        // if non-zero, set the desired target size in bytes.
                          // Takes precedence over the 'compression' parameter.
  float target_PSNR;      // if non-zero, specifies the minimal distortion to
                          // try to achieve. Takes precedence over target_size.
  int segments;           // maximum number of segments to use, in [1..4]
  int sns_strength;       // Spatial Noise Shaping. 0=off, 100=maximum.
  int filter_strength;    // range: [0 = off .. 100 = strongest]
  int filter_sharpness;   // range: [0 = off .. 7 = least sharp]
  int filter_type;        // filtering type: 0 = simple, 1 = strong (only used
                          // if filter_strength > 0 or autofilter > 0)
  int autofilter;         // Auto adjust filter's strength [0 = off, 1 = on]
  int alpha_compression;  // Algorithm for encoding the alpha plane (0 = none,
                          // 1 = compressed with WebP lossless). Default is 1.
  int alpha_filtering;    // Predictive filtering method for alpha plane.
                          //  0: none, 1: fast, 2: best. Default if 1.
  int alpha_quality;      // Between 0 (smallest size) and 100 (lossless).
                          // Default is 100.
  int pass;               // number of entropy-analysis passes (in [1..10]).

  int show_compressed;    // if true, export the compressed picture back.
                          // In-loop filtering is not applied.
  int preprocessing;      // preprocessing filter:
                          // 0=none, 1=segment-smooth, 2=pseudo-random dithering
  int partitions;         // log2(number of token partitions) in [0..3]. Default
                          // is set to 0 for easier progressive decoding.
  int partition_limit;    // quality degradation allowed to fit the 512k limit
                          // on prediction modes coding (0: no degradation,
                          // 100: maximum possible degradation).
  int emulate_jpeg_size;  // If true, compression parameters will be remapped
                          // to better match the expected output size from
                          // JPEG compression. Generally, the output size will
                          // be similar but the degradation will be lower.
  int thread_level;       // If non-zero, try and use multi-threaded encoding.
  int low_memory;         // If set, reduce memory usage (but increase CPU use).

  int near_lossless;      // Near lossless encoding [0 = max loss .. 100 = off
                          // (default)].
  int exact;              // if non-zero, preserve the exact RGB values under
                          // transparent area. Otherwise, discard this invisible
                          // RGB information for better compression. The default
                          // value is 0.

  int use_delta_palette;  // reserved for future lossless feature
  int use_sharp_yuv;      // if needed, use sharp (and slow) RGB->YUV conversion

  int qmin;               // minimum permissible quality factor
  int qmax;               // maximum permissible quality factor
  ...;
};
typedef struct WebPConfig WebPConfig;

struct WebPMemoryWriter {
  uint8_t* mem;
  size_t size;
  ...;
};
typedef struct WebPMemoryWriter WebPMemoryWriter;

struct WebPAnimEncoderOptions {
  int minimize_size;
  int kmin;
  int kmax;
  int allow_mixed;
  int verbose;
  ...;
};
typedef struct WebPAnimEncoderOptions WebPAnimEncoderOptions;

struct WebPAnimDecoderOptions {
  WEBP_CSP_MODE color_mode;
  int use_threads;
  ...;
};
typedef struct WebPAnimDecoderOptions WebPAnimDecoderOptions;

struct WebPAnimInfo {
  uint32_t canvas_width;
  uint32_t canvas_height;
  uint32_t loop_count;
  uint32_t bgcolor;
  uint32_t frame_count;
  ...;
};
typedef struct WebPAnimInfo WebPAnimInfo;

// Opaque objects
typedef struct WebPMux WebPMux;
typedef struct WebPAnimEncoder WebPAnimEncoder;
typedef struct WebPAnimDecoder WebPAnimDecoder;

int WebPPictureInit(WebPPicture* picture);
int WebPPictureAlloc(WebPPicture* picture);
int WebPPictureImportRGB(WebPPicture* picture, const uint8_t* rgb,
  int rgb_stride);
int WebPPictureImportRGBA(WebPPicture* picture, const uint8_t* rgba,
  int rgba_stride);
void WebPPictureFree(WebPPicture* picture);

int WebPInitDecoderConfig(WebPDecoderConfig* config);
VP8StatusCode WebPGetFeatures(const uint8_t* data, size_t data_size,
  WebPBitstreamFeatures* features);
VP8StatusCode WebPDecode(const uint8_t* data, size_t data_size,
  WebPDecoderConfig* config);
void WebPFreeDecBuffer(WebPDecBuffer* buffer);

int WebPConfigPreset(WebPConfig* config, WebPPreset preset, float quality);
int WebPValidateConfig(const WebPConfig* config);

int WebPEncode(const WebPConfig* config, WebPPicture* picture);

void WebPMemoryWriterInit(WebPMemoryWriter* writer);
int WebPMemoryWrite(const uint8_t* data, size_t data_size,
  const WebPPicture* picture);
void WebPMemoryWriterClear(WebPMemoryWriter* writer);

void WebPFree(void* ptr);

void WebPDataInit(WebPData* webp_data);
void WebPDataClear(WebPData* webp_data);

int WebPAnimEncoderOptionsInit(WebPAnimEncoderOptions* enc_options);
WebPAnimEncoder* WebPAnimEncoderNew(int width, int height,
  const WebPAnimEncoderOptions* enc_options);
int WebPAnimEncoderAdd(WebPAnimEncoder* enc, struct WebPPicture* frame,
  int timestamp_ms, const struct WebPConfig* config);
int WebPAnimEncoderAssemble(WebPAnimEncoder* enc, WebPData* webp_data);
void WebPAnimEncoderDelete(WebPAnimEncoder* enc);

int WebPAnimDecoderOptionsInit(WebPAnimDecoderOptions* dec_options);
WebPAnimDecoder* WebPAnimDecoderNew(const WebPData* webp_data,
  const WebPAnimDecoderOptions* dec_options);
int WebPAnimDecoderGetInfo(const WebPAnimDecoder* dec, WebPAnimInfo* info);
int WebPAnimDecoderHasMoreFrames(const WebPAnimDecoder* dec);
int WebPAnimDecoderGetNext(WebPAnimDecoder* dec, uint8_t** buf, int* timestamp);
void WebPAnimDecoderReset(WebPAnimDecoder* dec);
void WebPAnimDecoderDelete(WebPAnimDecoder* dec);
