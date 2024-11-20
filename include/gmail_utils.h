#ifndef GMAIL_UTILS_H
#define GMAIL_UTILS_H

/**
 * Estrutura que contém os detalhes de uma mensagem.
 */
typedef struct {
    char *from;
    char *subject;
    char *snippet;
} GmailMessageDetails;

/**
 * Extrai os detalhes de uma mensagem JSON.
 * 
 * @param message_json O JSON completo da mensagem.
 * @param details Ponteiro para a estrutura onde os detalhes serão armazenados.
 * @return 0 em caso de sucesso, -1 em caso de erro.
 */
int gmail_parse_message_details(const char *message_json, GmailMessageDetails *details);

/**
 * Codifica uma sequência de bytes no formato Base64.
 *
 * A codificação gerada não contém quebras de linha, sendo compatível com APIs 
 * que requerem Base64 contínuo, como o Gmail.
 * 
 * @param input  Ponteiro para os dados binários a serem codificados.
 * @param length Tamanho, em bytes, dos dados fornecidos no parâmetro `input`.
 * @return Uma string Base64 alocada dinamicamente ou NULL em caso de erro.
 *         O chamador é responsável por liberar a memória retornada com `free()`.
 */
char* gmail_base64_encode(const unsigned char *input, size_t length);

#endif // GMAIL_UTILS_H