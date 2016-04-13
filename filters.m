function [filts, envFilts] = filters(doScale, doPlot)
close all;

Fs = 8000;
FsBy2 = Fs/2;

% Same passband ripple and stopband attenuation for all filters
Rp = .2;       % Maximum passband ripple
Rs = 60;        % Minimum stop band attenuation
N = 20;

% centers = [ 240 360 480 600 720 840 1000 1150 1300 1450 1600 1800 2000 2200 2400 2700];
% widths = [120 120 120 120 120 120 150 150 150 150 150 200 200 200 200 300];

centers = [ 200 400 800 1600];
%centers = 240:(2000-240)/5:2000
widths = centers;

filts = zeros(length(centers), N+1);
envFilts = zeros(length(centers), N+1);
% Band Pass 
for i = 1:length(centers)
    fcenter = centers(i);
    fwidth = widths(i);
    Wp = [fcenter-(fwidth/2), fcenter+(fwidth/2)]/FsBy2;
    bpf = fir1(N, Wp, 'bandpass',chebwin(N+1, Rs));
    Wpl = (fcenter*0.75)/FsBy2;
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
        axis([0 3300 -100, 1]);
        xlabel('Frequency(Hz)');
        ylabel('Magnitude(dB)');
        if(i < length(centers));
            figure();
        end
    end
end

end