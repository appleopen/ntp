/*
 *  libsntp.h
 *  ntp
 *
 *  Copyright 2011 Apple Inc. All rights reserved.
 *
 */

#ifndef __LIBSNTP_H__
#define __LIBSNTP_H__

__BEGIN_DECLS

#define LIBSNTP_API_VERSION 20140531

/*!
 * @enum sntp_query_result_t
 *
 * @abstract
 * Describes the result of the requested NTP query.
 *
 * Because a query may involve contacting a number of servers, the error
 * reported is an aggregate of the results for contacting servers.  The
 * most specific error possible will be reported.
 *
 * If any server returns a "kiss of death" or rate limit response, the appropriate
 * result code will be returned for the operation.  However, there may still be
 * results available.
 */
typedef enum {
	SNTP_RESULT_SUCCESS = 0,
	SNTP_RESULT_FAILURE_INTERNAL,
	SNTP_RESULT_FAILURE_DNS,
	SNTP_RESULT_FAILURE_SERVER_UNUSABLE,
	SNTP_RESULT_FAILURE_AUTHORIZATION,
	SNTP_RESULT_FAILURE_PACKET_UNUSABLE,
	SNTP_RESULT_FAILURE_SERVER_RATE_LIMIT,
	SNTP_RESULT_FAILURE_SERVER_KISSOFDEATH,
	SNTP_RESULT_FAILURE_CANNOT_BIND_SOCKET
} sntp_query_result_t;

/*!
 * @typedef	sntp_query_result_handler_t
 * A block to be invoked when an NTP time is retrieved.
 *
 * @param query_result
 * The result for the most recently completed query.
 *
 * @param time
 * If successful, the time retrieved from the NTP server.
 *
 * @param delay
 * If successful, the time taken, in seconds, for the packet to be sent and a response received from the.
 * server.  This does not include the processing time on the other end (compensated for in the response).
 *
 * @param dispersion
 * The maximum error of the remote clock.
 *
 * @param more_servers
 * True if there are more servers that can be queried; false otherwise.
 *
 * @result
 * True if the library should query the next server; false otherwise. The result is ignored if more_servers is false.
 */
typedef bool (^sntp_query_result_handler_t) (sntp_query_result_t query_result, struct timeval time, double delay, double dispersion, bool more_servers);

/*!
 * @typedef	sntp_query_extended_result_handler_t
 * A block to be invoked when an NTP time is retrieved.
 *
 * @param query_result
 * The result for the most recently completed query.
 *
 * @param mach_timestamp
 * The mach_absolute_time() of the packet receipt.
 *
 * @param time
 * If successful, the time retrieved from the NTP server.
 *
 * @param delay
 * If successful, the time taken, in seconds, for the packet to be sent and a response received from the.
 * server.  This does not include the processing time on the other end (compensated for in the response).
 *
 * @param dispersion
 * The maximum error of the remote clock.
 *
 * @param more_servers
 * True if there are more servers that can be queried; false otherwise.
 *
 * @param ip_addr
 * The IP address of the result.
 *
 * @param port
 * The decimal port of the result.
 *
 * @param retry_attempts
 * Number of retries before result was successful (or gave up if failed)
 *
 * @result
 * True if the library should query the next server; false otherwise. The result is ignored if more_servers is false.
 */
typedef bool (^sntp_query_extended_result_handler_t) (sntp_query_result_t query_result, uint64_t mach_timestamp, struct timeval time, double delay, double dispersion, bool more_servers, const char *ip_addr, const char *port, int retry_attempts);

/*!
 * @function sntp_query
 *
 * @abstract
 * Initiates a query of the specified NTP server.
 *
 * @param host
 * The NTP hostname that is to be queried.
 *
 * @param result_handler
 * The handler to be invoked once the request is complete.
 *
 * @discussion
 * Note that this API will make no effort to bring up the PDP context on embedded platforms.  If you want
 * this API to function over cellular networks, it is the caller's responsiblity to bring up the PDP context.
 *
 * The callback will be called once for each server resolved from the given hostname. Return false from the
 * handler to stop further lookups.
 *
 * No guarantees are made as to the execution context of the callback.
 */
void
sntp_query(char *host, bool use_service_port, sntp_query_result_handler_t result_handler);


/*!
 * @function sntp_query_extended
 *
 * @abstract
 * Initiates a query of the specified NTP server.
 *
 * @param hostname
 * The NTP hostname that is to be queried.
 *
 * @param servname
 * The service or decimal port to contact as suitable for use by getaddrinfo. May pass NULL for the default "123"/"ntp".
 *
 * @param use_service_port
 * Should the library bind to port 123 to receive the reply
 *
 * @param attempts
 * Number of attempts to reach each server before continuing
 *
 * @param attempt_timeout
 * Time to wait for each attempt
 *
 * @param result_handler
 * The handler to be invoked once the request is complete.
 *
 * @discussion
 * Note that this API will make no effort to bring up the PDP context on embedded platforms.  If you want
 * this API to function over cellular networks, it is the caller's responsiblity to bring up the PDP context.
 *
 * The callback will be called once for each server resolved from the given hostname. Return false from the
 * handler to stop further lookups.
 *
 * No guarantees are made as to the execution context of the callback.
 */
void
sntp_query_extended(const char *hostname, const char *servname, bool use_service_port, int attempts, struct timeval attempt_timeout, sntp_query_extended_result_handler_t result_handler);

__END_DECLS

#endif /* __LIBSNTP_H__ */
