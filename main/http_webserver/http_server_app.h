#ifndef HTTP_SERVER_APP_H
#define HTTP_SERVER_APP_H

/******************************************************************************/

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/



/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/* Handle data receive from user. */
typedef void (*http_handle_data_t)(char *buf);


/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/



/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/



/******************************************************************************/
/*                                FUNCTIONS                                   */
/******************************************************************************/

/*!
 * @brief  start webserver
 */
void HTTP_startWebserver(void);

/*!
 * @brief  stop webserver
 */
void HTTP_stopWebserver(void);

/*!
 * @brief register callback to use in the app main.
 */
void HTTP_setCallback(void* callback);

/*!
 * @brief Deserialize wifi info receive from user.
 */
int HTTP_jsonLoginDeserialize(char *input_data, char *user, char *password);

#endif /* HTTP_SERVER_APP_H */
