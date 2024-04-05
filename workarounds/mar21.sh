curl -O https://s3.amazonaws.com/fast-ai-imageclas/imagenette2-320.tgz
git clone https://github.com/GustavMH/cfa-raw
git clone https://github.com/schoolpost/PiDNG
mkdir cfa-raw/notebooks/pidng
cp -r PiDNG/src/pidng/ cfa-raw/notebooks/pidng/
rm -rf cfa-raw/ PiDNG/
git clone https://github.com/GustavMH/cfa-raw
module load libjpeg/9e
python3.11 -m pip install numpy torch Pillow
# Pillow wont build on RHEL, it cant find libjpeg
# Use python3.11 to get a binary from PyPi
# setup pnmtopng and dcraw
wget https://sourceforge.net/projects/png-mng/files/pnmtopng/2.39/pnmtopng-2.39-x86-linux.zip/download
curl -O https://www.dechifro.org/dcraw/dcraw.c
gcc -o dcraw -O4 dcraw.c -lm -DNODEPS
export ds_path=/home/zvq211/ds_proc/
sbatch ~/cfa-raw/notebooks/demosaic.sh
# the ld linker elsewhere
# we'll have to compile it ourselves
