create or replace view view_last_run as
SELECT 'Current Run Number' AS NAME,
       TO_CHAR ( MAX ( RUNNUMBER ) ) AS VALUE
  FROM FILES_CREATED
    WHERE PRODUCER='StorageManager'
  UNION SELECT 'Number of files' AS NAME,
       TO_CHAR ( COUNT ( FILENAME ) ) AS VALUE
  FROM FILES_CREATED
 WHERE RUNNUMBER = ( SELECT MAX ( RUNNUMBER )
                       FROM FILES_CREATED WHERE PRODUCER='StorageManager' )
    AND PRODUCER='StorageManager'
  UNION SELECT 'Number of closed files' AS NAME,
       TO_CHAR ( COUNT ( * ) ) AS VALUE
  FROM FILES_CREATED JOIN FILES_INJECTED on FILES_CREATED.FILENAME = FILES_INJECTED.FILENAME
 WHERE RUNNUMBER = ( SELECT MAX ( RUNNUMBER )
                       FROM FILES_CREATED WHERE PRODUCER='StorageManager')
    AND PRODUCER='StorageManager'
 UNION SELECT 'Number of hosts' AS NAME,
       TO_CHAR ( COUNT ( DISTINCT HOSTNAME ) ) AS VALUE
  FROM FILES_CREATED
 WHERE RUNNUMBER = ( SELECT MAX ( RUNNUMBER )
                       FROM FILES_CREATED WHERE PRODUCER='StorageManager' )
    AND PRODUCER='StorageManager'
 UNION SELECT 'Number of streams' AS NAME,
       TO_CHAR ( COUNT ( DISTINCT STREAM ) ) AS VALUE
  FROM FILES_CREATED
 WHERE RUNNUMBER = ( SELECT MAX ( RUNNUMBER )
                       FROM FILES_CREATED WHERE PRODUCER='StorageManager' )
    AND PRODUCER='StorageManager'
 UNION SELECT 'Number of luminosity sections' AS NAME,
       TO_CHAR ( COUNT ( DISTINCT LUMISECTION ) ) AS VALUE
  FROM FILES_CREATED
 WHERE RUNNUMBER = ( SELECT MAX ( RUNNUMBER )
                       FROM FILES_CREATED WHERE PRODUCER='StorageManager' )
    AND PRODUCER='StorageManager'
 UNION SELECT 'Total size of closed files' AS NAME,
       TO_CHAR ( ROUND ( SUM ( FILESIZE ) / 1073741824,
                         2 ) ) || ' GB' AS VALUE
  FROM FILES_CREATED JOIN FILES_INJECTED on FILES_CREATED.FILENAME = FILES_INJECTED.FILENAME
 WHERE RUNNUMBER = ( SELECT MAX ( RUNNUMBER )
                       FROM FILES_CREATED WHERE PRODUCER='StorageManager' )
    AND PRODUCER='StorageManager'
 UNION SELECT 'Run Start Time' AS NAME,
       TO_CHAR ( MIN ( CTIME ), 'YYYY/MM/DD HH24:MI' ) AS VALUE
  FROM FILES_CREATED
 WHERE RUNNUMBER = ( SELECT MAX ( RUNNUMBER )
                       FROM FILES_CREATED WHERE PRODUCER='StorageManager' )
    AND PRODUCER='StorageManager'
 UNION SELECT 'Number of files (count > 0)' AS NAME,
       TO_CHAR ( COUNT ( * ) ) AS VALUE
  FROM FILES_CREATED
 WHERE RUNNUMBER = ( SELECT MAX ( RUNNUMBER )
                       FROM FILES_CREATED WHERE PRODUCER='StorageManager' )
   AND COUNT <> 0
    AND PRODUCER='StorageManager'
 UNION SELECT 'Number of files (safety > 0)' AS NAME,
       TO_CHAR ( COUNT ( * ) ) AS VALUE
  FROM FILES_CREATED JOIN FILES_TRANS_COPIED on FILES_CREATED.FILENAME = FILES_TRANS_COPIED.FILENAME
 WHERE RUNNUMBER = ( SELECT MAX ( RUNNUMBER )
                       FROM FILES_CREATED WHERE PRODUCER='StorageManager' )
    AND PRODUCER='StorageManager'
 UNION SELECT 'Number of files (safety > 99)' AS NAME,
       TO_CHAR ( COUNT ( * ) ) AS VALUE
  FROM FILES_CREATED JOIN FILES_TRANS_CHECKED on FILES_CREATED.FILENAME = FILES_TRANS_CHECKED.FILENAME
 WHERE RUNNUMBER = ( SELECT MAX ( RUNNUMBER )
                       FROM FILES_CREATED WHERE PRODUCER='StorageManager' )
    AND PRODUCER='StorageManager'
 UNION SELECT 'Total size of safe (safety > 99) files' AS NAME,
       TO_CHAR ( ROUND ( SUM ( FILESIZE ) / 1073741824,
                         2 ) ) || ' GB' AS VALUE
  FROM FILES_CREATED JOIN FILES_INJECTED on FILES_CREATED.FILENAME = FILES_INJECTED.FILENAME
                     JOIN FILES_TRANS_CHECKED on FILES_CREATED.FILENAME = FILES_TRANS_CHECKED.FILENAME
 WHERE RUNNUMBER = ( SELECT MAX ( RUNNUMBER )
                       FROM FILES_CREATED WHERE PRODUCER='StorageManager' )
    AND PRODUCER='StorageManager'
 UNION SELECT 'Total size of safe (safety > 0) files' AS NAME,
       TO_CHAR ( ROUND ( SUM ( FILESIZE ) / 1073741824,
                         2 ) ) || ' GB' AS VALUE
  FROM FILES_CREATED JOIN FILES_INJECTED on FILES_CREATED.FILENAME = FILES_INJECTED.FILENAME
                     JOIN FILES_TRANS_COPIED on FILES_CREATED.FILENAME = FILES_TRANS_COPIED.FILENAME
 WHERE RUNNUMBER = ( SELECT MAX ( RUNNUMBER )
                       FROM FILES_CREATED WHERE PRODUCER='StorageManager' )
    AND PRODUCER='StorageManager'
 UNION SELECT 'SetupLabel' AS NAME,
       TO_CHAR ( SETUPLABEL ) AS VALUE
  FROM FILES_CREATED
 WHERE RUNNUMBER = ( SELECT MAX ( RUNNUMBER )
                       FROM FILES_CREATED WHERE PRODUCER='StorageManager' )
    AND PRODUCER='StorageManager'
UNION SELECT 'Rate average' AS NAME,
  RATE_AVERAGE(MAX(RUNNUMBER)) AS VALUE
   FROM FILES_CREATED
     WHERE PRODUCER='StorageManager';

grant select on view_last_run to public;
