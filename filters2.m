function filts = filters2(nFilters, N, doScale, doPlot)
Fs = 48000;
FsBy2 = Fs/2;
% Same passband ripple and stopband attenuation for all filters
Rs = 50;        % Minimum stop band attenuation
%N = 200;
% centers = [ 240 360 480 600 720 840 1000 1150 1300 1450 1600 1800 2000 2200 2400 2700];
% widths = [40 40 40 40 40 40 40 40 40 40 40 60 60 60 60 60];

%centers = [ 200 400 800 1600];
startfreq = 240;
stopfreq = 3500;
bWidth = (stopfreq - startfreq) / nFilters;
centers = (startfreq + (bWidth/2)) : bWidth : (stopfreq - (bWidth/2));
widths = ones(1,nFilters) * bWidth / 2;

filts = zeros(length(centers), N+1);
% Band Pass
for i = 1:length(centers)
    fcenter = centers(i);
    fwidth = widths(i);
    Wp = [fcenter-fwidth/2, fcenter+fwidth/2]/FsBy2;
    bpf = fir1(N, Wp, 'bandpass',chebwin(N+1, Rs));
    if(doScale)
        filts(i, 1:N+1) = round(bpf .* 32767);
    else
        filts(i,1:N+1) = bpf;
    end
    
    if(doPlot)
        [H1, W1] = freqz(bpf);
        plot(W1*FsBy2/pi, 20*log10(abs(H1)),'b')
        axis([0 3300 -100, 1]);
        xlabel('Frequency(Hz)');
        ylabel('Magnitude(dB)');
        if(i < length(centers));
            hold on;
        end
    end
end

end
