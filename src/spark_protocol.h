#include "coap.h"
#include "tropicssl/aes.h"

namespace ProtocolState {
  enum Enum {
    READ_NONCE
  };
}

namespace ChunkReceivedCode {
  enum Enum {
    OK = 0x44,
    BAD = 0x80
  };
}

class SparkProtocol
{
  public:
    SparkProtocol();
    ~SparkProtocol();

    int init(const unsigned char *private_key,
             const unsigned char *pubkey,
             const unsigned char *signed_encrypted_credentials);
    CoAPMessageType::Enum received_message(unsigned char *buf, int length);
    void hello(unsigned char *buf);
    void key_changed(unsigned char *buf, unsigned char token);
    void function_return(unsigned char *buf, unsigned char token,
                         int return_value);
    void variable_value(unsigned char *buf, unsigned char token,
                        bool return_value);
    void variable_value(unsigned char *buf, unsigned char token,
                        int return_value);
    void variable_value(unsigned char *buf, unsigned char token,
                        double return_value);
    void variable_value(unsigned char *buf, unsigned char token,
                        const void *return_value, int length);
    void event(unsigned char *buf,
               const char *event_name,
               int event_name_length);
    void event(unsigned char *buf,
               const char *event_name,
               int event_name_length,
               const char *data,
               int data_length);
    void chunk_received(unsigned char *buf, unsigned char token,
                        ChunkReceivedCode::Enum code);
    void update_ready(unsigned char *buf, unsigned char token);
    void description(unsigned char *buf, unsigned char token,
                     const char **function_names, int num_functions);

    /********** Queue **********/
    const int QUEUE_SIZE;
    int queue_bytes_available();
    int queue_push(const char *src, int length);
    int queue_pop(char *dst, int length);

    /********** State Machine **********/
    ProtocolState::Enum state();

  private:
    aes_context aes;
    unsigned char key[16];
    unsigned char iv[16];
    unsigned char salt[8];
    unsigned short _message_id;

    unsigned short next_message_id();
    void encrypt(unsigned char *buf, int length);
    void separate_response(unsigned char *buf, unsigned char token, unsigned char code);

    /********** Queue **********/
    char *queue;
    const char *queue_mem_boundary;
    char *queue_front;
    char *queue_back;
};
