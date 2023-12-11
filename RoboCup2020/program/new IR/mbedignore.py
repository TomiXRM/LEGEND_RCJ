Import("env")
import os
import glob

srcs = glob.glob(os.getcwd() + '/mbedignore/*')
print(srcs)
dst_basedir = env.Split(env['PROJECT_PACKAGES_DIR'])[0] + '/framework-mbed'

def clean_mbedignore(source, target, env):
    print('Clean .mbedignore')

    for src in srcs:
        dst_dir = dst_basedir + '/' + os.path.basename(src)
        dst = dst_dir +'/.mbedignore'

        if os.path.exists(dst) == True:
            os.remove(dst)
            print('symlink deleted : ' + dst)


def mbedignore():
    for src in srcs:
        dst_dir = dst_basedir + '/' + os.path.basename(src)
        dst = dst_dir +'/.mbedignore'

        if os.path.exists(dst) == True:
            os.remove(dst)
            print('symlink deleted : ' + dst)

        if os.path.exists(dst_dir) == True:
            os.symlink(src, dst)
            print('symlink created : src = ' + src + '->' + dst)


mbedignore()
env.AddPostAction("checkprogsize", clean_mbedignore)
