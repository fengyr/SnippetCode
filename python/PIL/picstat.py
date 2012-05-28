#!encoding=utf8
import math
import Image
import ImageStat
import ImageChops

def CalcMES(imge1, imge2):
    """
        RMS = sqr[ sum[pixel^2] / ImageSize ]
        MSE = RMS^2 = sum[ (recpixel - orgpixel)^2 ] / ImageSize
        PSNR=10*log10(255^2/MSE)
        """

    stat1 = ImageStat.Stat(imge1)
    stat2 = ImageStat.Stat(imge2)

    #print imge1.mode
    extrema = stat1.extrema

    MSE = []                              
    PSNR = 0.0
    for i in range(3):                                        
        #print float(stat2.rms[i]) ,float(stat1.rms[i])
        mse = float(stat2.rms[i])**2 - float(stat1.rms[i])**2
        psnr = 10*math.log10(255*255/abs(mse))                    
        print '%d => PSNR: %f  MSE: %f' % (i, psnr, mse)                   
        MSE.append(mse)

    mse = MSE[0] + MSE[1] + MSE[2]
    PSNR = 10*math.log10(255*255/mse)                    
    print 'all => PSNR: %f  MSE: %f' % (PSNR, mse) 
    

if __name__ == '__main__':
    im1 = Image.open('ybr1.jpg')
    im2 = Image.open('ybr2.jpg')

    CalcMES(im1, im2)
