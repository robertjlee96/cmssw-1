create or replace view view_last_runs as
SELECT "RUN_NUMBER",
       "START_TIME",
       "SETUPLABEL",
       "TOTAL_SIZE",
       "NEVTS",
       "NFILES",
       "RATE2D_AVG",
       "RATE2T_AVG",
       "N_OPEN",
       "N_CLOSED",
       "N_SAFE0",
       "N_SAFE99",
       "N_DELETED"
  FROM ( SELECT TO_CHAR ( RUNNUMBER ) AS RUN_NUMBER,
		TO_CHAR ( MIN ( CREATED_TIME ),
			  'YYYY/MM/DD HH24:MI' ) AS START_TIME,
		TO_CHAR ( MIN ( SETUPLABEL ) ) AS SETUPLABEL,
		TO_CHAR ( ROUND ( SUM ( FILESIZE ) / 1073741824,
				  2 ) ) || ' GB' AS TOTAL_SIZE,
		TO_CHAR ( SUM ( NEVENTS ) ) AS NEVTS,
		TO_CHAR ( COUNT ( CREATED_TIME ) ) AS NFILES,
		RATE2D_AVERAGE ( RUNNUMBER ) AS RATE2D_AVG,
		RATE2T_AVERAGE ( RUNNUMBER ) AS RATE2T_AVG,
		TO_CHAR ( COUNT ( CREATED_TIME ) - COUNT ( INJECTED_TIME ) ) AS N_OPEN,
		TO_CHAR ( COUNT ( INJECTED_TIME ) ) AS N_CLOSED,
		TO_CHAR ( COUNT ( NEW_TIME      ) ) AS N_SAFE0,
		TO_CHAR ( COUNT ( CHECKED_TIME  ) ) AS N_SAFE99,
		TO_CHAR ( COUNT ( DELETED_TIME  ) ) AS N_DELETED
   FROM ( SELECT RUNNUMBER, SETUPLABEL, FILESIZE, NEVENTS,
                 CREATED_TIME, INJECTED_TIME, NEW_TIME, CHECKED_TIME, DELETED_TIME,
			 DENSE_RANK ( )
		    OVER ( ORDER BY RUNNUMBER DESC
			   NULLS LAST ) RUNNUMBER_rank
		    FROM FILES_INFO RUNNUMBER_rank ) FILES_INFO
	  WHERE RUNNUMBER_rank <= 10
	  GROUP BY RUNNUMBER )
 ORDER BY 1 DESC;
        
grant select on view_last_runs to public;
