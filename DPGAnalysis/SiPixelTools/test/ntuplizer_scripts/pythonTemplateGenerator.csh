#!/bin/csh
set limit=200
echo "Will write $limit files in each template ..."

rm -f template*

sed '1,/STRINGTOCHANGE/d'<pixelefficiency_template_cfg.py>pixelefficiency_templateB_cfg.py
sed '/STRINGTOCHANGE/,$ d' <pixelefficiency_template_cfg.py>pixelefficiency_templateA_cfg.py

cp pixelefficiency_templateA_cfg.py template1_pixelefficiency_cfg.py

set i=1
set linenumber=0
set totallines=0

foreach fileline ( `more filelist.txt` )
  @ totallines++
end
   
foreach fileline ( `more filelist.txt` )
  
  @ linenumber++
  if (($linenumber != $limit) && ($linenumber != $totallines)) then
  echo $fileline,>>template${i}_pixelefficiency_cfg.py
  endif
  
  if (($linenumber == $limit) && ($linenumber != $totallines)) then
  echo $fileline>>template${i}_pixelefficiency_cfg.py
  cat pixelefficiency_templateB_cfg.py>>template${i}_pixelefficiency_cfg.py
  echo template${i}_pixelefficiency_cfg.py done !
  @ i++
  cp pixelefficiency_templateA_cfg.py template${i}_pixelefficiency_cfg.py
  set linenumber=0
  endif
  
  if (($linenumber == $totallines)) then
  echo $fileline>>template${i}_pixelefficiency_cfg.py
  cat pixelefficiency_templateB_cfg.py>>template${i}_pixelefficiency_cfg.py
  echo template${i}_pixelefficiency_cfg.py done !
  endif 
end

rm -f pixelefficiency_templateB_cfg.py
rm -f pixelefficiency_templateA_cfg.py
