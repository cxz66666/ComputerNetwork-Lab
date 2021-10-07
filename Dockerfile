FROM alpine
CMD cmake .
EXPOSE 4611
WORKDIR /app
COPY ./
