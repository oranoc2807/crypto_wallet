idf_component_register(
    SRCS 
        "main.c"
        "cryptography/key_storage.c"
        "falcon/falcon.c"
        "falcon/codec.c"
        "falcon/common.c"
        "falcon/fft.c"
        "falcon/fpr.c"
        "falcon/keygen.c"
        "falcon/rng.c"
        "falcon/shake.c"
        "falcon/sign.c"
        "falcon/speed.c"
        "falcon/test_falcon.c"
        "falcon/vrfy.c"
    INCLUDE_DIRS 
        "."
        "cryptography"
        "falcon"
)


