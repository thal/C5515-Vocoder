function [filts, envFilts] = filters(nFilters, N, doScale, doPlot)
Fs = 8000;
FsBy2 = Fs/2;

Rs = 50;        % Minimum stop band attenuation

startfreq = 240;
stopfreq = 3500;
bWidth = (stopfreq - startfreq) / nFilters;
centers = (startfreq + (bWidth/2)) : bWidth : (stopfreq - (bWidth/2));
widths = ones(1,nFilters) * bWidth /2;

filts = zeros(length(centers), N+1);
envFilts = zeros(length(centers), N+1);

for i = 1:length(centers)
    fcenter = centers(i);
    fwidth = widths(i);
    Wp = [fcenter-(fwidth/2), fcenter+(fwidth/2)]/FsBy2;
    bpf = fir1(N, Wp, 'bandpass',chebwin(N+1, Rs));
    Wpl = (fcenter - (fwidth * 0.5))/FsBy2;
    lpf = fir1(N, Wpl, chebwin(N+1,Rs));
    if(doScale)
        filts(i, 1:N+1) = round(bpf .* 32767);
        envFilts(i, 1:N+1) = round(lpf .* 32767);
    else
        filts(i, 1:N+1) = bpf;
        envFilts(i, 1:N+1) = lpf;
    end
    
    if(doPlot)
        [H1, W1] = freqz(bpf);
        
        plot(W1*FsBy2/pi, 20*log10(abs(H1)),'b')
        axis([0 4000 -80, 1]);
        xlabel('Frequency(Hz)');
        ylabel('Magnitude(dB)');
        if(i < length(centers));
            hold on;
        end
    end
end

end