g++ main.cpp -o main.o -lmysqlcppconn

CREATE USER 'admin'@'localhost' IDENTIFIED BY 'admin';
GRANT ALL PRIVILEGES ON lista5.* TO 'admin'@'localhost';
FLUSH PRIVILEGES;

CREATE USER 'user'@'localhost' IDENTIFIED BY 'user';


CREATE TABLE User (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nick VARCHAR(100) UNIQUE,
    firstName VARCHAR(100),
    lastName VARCHAR(100),
    email VARCHAR(255) UNIQUE,
    password VARCHAR(255) NOT NULL
);

CREATE TABLE Author (
    id INT AUTO_INCREMENT PRIMARY KEY,
    firstName VARCHAR(100),
    lastName VARCHAR(100)
);

CREATE TABLE Category (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100)
);

CREATE TABLE Book (
    id INT AUTO_INCREMENT PRIMARY KEY,
    title VARCHAR(255) NOT NULL,
    authorId INT,
    isbn VARCHAR(20) NOT NULL,
    year INT,
    CHECK (isbn REGEXP '^[0-9]{13}$'),
    FOREIGN KEY (authorId) REFERENCES Author(id)
);

CREATE TABLE Borrow (
    id INT AUTO_INCREMENT PRIMARY KEY,
    userId INT,
    bookId INT,
    borrowDate DATE,
    returnDate DATE,
    FOREIGN KEY (userId) REFERENCES User(id),
    FOREIGN KEY (bookId) REFERENCES Book(id)
);

CREATE TABLE Category_Book (
    bookId INT,
    categoryId INT,
    FOREIGN KEY (bookId) REFERENCES Book(id),
    FOREIGN KEY (categoryId) REFERENCES Category(id)
);

DELIMITER ;

DELIMITER $$

CREATE TRIGGER checkYear
BEFORE INSERT ON Book
FOR EACH ROW
BEGIN
    IF NEW.year > YEAR(CURDATE()) THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Release date cannot be in the past';
    END IF;
END $$

DELIMITER ;

//procedura

DELIMITER $$

CREATE PROCEDURE AddBook(
    IN _title VARCHAR(255),
    IN _authorFName VARCHAR(255),
    IN _authorLName VARCHAR(255),
    IN _year INT,
    IN _isbn VARCHAR(20)
)
BEGIN
    DECLARE number INT;

    SELECT COUNT(*)
    INTO number
    FROM Author
    WHERE firstName=_authorFName AND lastName=_authorLName;

    IF number = 0 THEN
        INSERT INTO Author (firstName, lastName)
        VALUES (_authorFName, _authorLName);
    END IF;

    SELECT id
    INTO number
    FROM Author
    WHERE firstName=_authorFName AND lastName=_authorLName
    LIMIT 1;

    INSERT INTO Book (title, authorID, isbn, year)
    VALUES (_title, number, _isbn, _year);
END $$

DELIMITER ;


call AddBook("bookTitle","authorF","authorL",2137,"abcdefg");
call AddBook("bookTitle","authorF","authorL",1000,"1111111111111");

//procedura 2

DELIMITER $$

CREATE PROCEDURE UpdateBook(
    IN _bookId INT,
    IN _title VARCHAR(255),
    IN _authorFName VARCHAR(255),
    IN _authorLName VARCHAR(255),
    IN _year INT,
    IN _isbn VARCHAR(20)
)
BEGIN
    DECLARE authorId INT;
    DECLARE number INT;

    SELECT COUNT(*)
    INTO number
    FROM Author
    WHERE firstName = _authorFName AND lastName = _authorLName;

    IF number = 0 THEN
        INSERT INTO Author (firstName, lastName)
        VALUES (_authorFName, _authorLName);
    END IF;

    SELECT id
    INTO authorId
    FROM Author
    WHERE firstName = _authorFName AND lastName = _authorLName
    LIMIT 1;

    UPDATE Book
    SET title = _title,
        authorId = authorId,
        isbn = _isbn,
        year = _year
    WHERE id = _bookId;

END $$

DELIMITER ;


//trigery do autorów

DELIMITER $$

CREATE TRIGGER delAuthort
AFTER DELETE ON Book
FOR EACH ROW
BEGIN
    DECLARE number INT;
    SELECT COUNT(*) INTO number
    FROM Author
    WHERE Author.id=OLD.authorId;

    IF number=0 THEN
        DELETE FROM Author
        WHERE Author.id=OLD.authorId;
    END IF;
END $$

DELIMITER ;


DELIMITER $$

CREATE TRIGGER delAuthor
AFTER UPDATE ON Book
FOR EACH ROW
BEGIN
    DECLARE number INT;
    SELECT COUNT(*) INTO number
    FROM Author
    WHERE Author.id=OLD.authorId;

    IF number=0 THEN
        DELETE FROM Author
        WHERE Author.id=OLD.authorId;
    END IF;
END $$

DELIMITER ;


//user valid

DELIMITER $$

CREATE TRIGGER validateUser
BEFORE INSERT ON User
FOR EACH ROW
BEGIN
    IF NEW.email NOT REGEXP '^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$' THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Invalid email format';
    END IF;
END$$

DELIMITER ;

//autoryzacja

DELIMITER $$

CREATE FUNCTION userAuth(
    user VARCHAR(255),
    pass VARCHAR(255)
) RETURNS BOOLEAN
READS SQL DATA
BEGIN
    DECLARE number INT;

    SELECT COUNT(*) INTO number FROM User
    WHERE User.password = pass AND User.nick = user;

    IF number > 0 THEN
        RETURN TRUE;
    ELSE
        RETURN FALSE;
    END IF;
END $$

DELIMITER ;

GRANT EXECUTE ON PROCEDURE userSearch TO 'user'@'localhost';
GRANT EXECUTE ON FUNCTION userAuth TO 'user'@'localhost';   



DELIMITER $$

CREATE TRIGGER before_user_delete
BEFORE DELETE ON User
FOR EACH ROW
BEGIN
    DELETE FROM Borrow WHERE userId = OLD.id;
END $$

DELIMITER ;

DELIMITER $$

CREATE TRIGGER before_book_delete
BEFORE DELETE ON Book
FOR EACH ROW
BEGIN
    DELETE FROM Borrow WHERE bookId = OLD.id;
END $$

DELIMITER ;

DELIMITER $$

CREATE TRIGGER genreDelete
BEFORE DELETE ON Book
FOR EACH ROW
BEGIN
    DELETE FROM Category_Book WHERE bookId = OLD.id;
END $$

DELIMITER ;


DELIMITER $$
CREATE FUNCTION SplitString(str VARCHAR(255), delim VARCHAR(1), pos INT) 
RETURNS VARCHAR(255)
DETERMINISTIC
BEGIN
    DECLARE output VARCHAR(255);
    SET output = SUBSTRING_INDEX(SUBSTRING_INDEX(str, delim, pos), delim, -1);
    RETURN output;
END $$
DELIMITER ;


drop Procedure borrowBooks;

DELIMITER $$
CREATE PROCEDURE borrowBooks(IN nickIn VARCHAR(100),IN inputList VARCHAR(255))
BEGIN

    DECLARE i INT DEFAULT 1;
    DECLARE current_value VARCHAR(255);
    DECLARE bookCount INT;
    DECLARE user INT;

    START TRANSACTION;

    SET current_value = SplitString(inputList, ',', i);
    SET i = i + 1;

    WHILE LENGTH(TRIM(current_value))!=0 DO

        

        SELECT COUNT(*) INTO bookCount 
        FROM Book
        LEFT JOIN Borrow ON Borrow.bookId=Book.id
        WHERE Book.title=TRIM(current_value)
        AND (Book.id NOT IN
        (
            SELECT Book.id
            FROM Book 
            INNER JOIN Borrow ON Borrow.bookId=Book.id
            WHERE Borrow.returnDate IS NULL
        )
        OR Book.id IS NULL);


        IF bookCount=0 THEN
            ROLLBACK;
            SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'no book available'; 
        END IF;

        SELECT MIN(Book.id) INTO bookCount 
        FROM Book
        LEFT JOIN Borrow ON Borrow.bookId=Book.id
        WHERE (Borrow.userId IS NULL OR Borrow.returnDate IS NOT NULL) AND Book.title=TRIM(current_value);

        SELECT User.id INTO user 
        FROM User
        WHERE User.nick=nickIn;

        INSERT INTO Borrow(bookId,userId,borrowDate)
        VALUES (bookCount,user,CURDATE());
        
        SET current_value = SplitString(inputList, ',', i);
        SET i = i + 1;
    END WHILE;
    COMMIT;
END $$
DELIMITER ;

drop Procedure returnBooks;

DELIMITER $$
CREATE PROCEDURE returnBooks(IN nickIn VARCHAR(100),IN inputList VARCHAR(255))
BEGIN

    DECLARE i INT DEFAULT 1;
    DECLARE current_value VARCHAR(255);
    DECLARE bookCount INT;
    DECLARE user INT;
    DECLARE borrow INT;

    START TRANSACTION;

    SET current_value = SplitString(inputList, ',', i);
    SET i = i + 1;

    WHILE LENGTH(TRIM(current_value))!=0 DO

        SELECT COUNT(*) INTO bookCount 
        FROM Book
        INNER JOIN Borrow ON Borrow.bookId=Book.id
        INNER JOIN User ON Borrow.userId=User.id 
        WHERE User.nick=nickIn AND TRIM(current_value)=Book.title AND Borrow.returnDate IS NULL;

        IF bookCount=0 THEN
            ROLLBACK;
            SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'wrong titles'; 
        END IF;

        SELECT MIN(Book.id) INTO bookCount 
        FROM Book
        INNER JOIN Borrow ON Borrow.bookId=Book.id
        INNER JOIN User ON Borrow.userId=User.id 
        WHERE User.nick=nickIn AND TRIM(current_value)=Book.title AND Borrow.returnDate IS NULL;

        SELECT MIN(User.id) INTO user 
        FROM User
        WHERE User.nick=nickIn;

        SELECT MIN(Borrow.id) INTO borrow
        FROM Borrow
        WHERE Borrow.userId=user AND Borrow.bookId=bookCount AND Borrow.returnDate IS NULL;

        UPDATE Borrow
        SET returnDate=CURDATE()
        WHERE Borrow.id=borrow;
        
        SET current_value = SplitString(inputList, ',', i);
        SET i = i + 1;
    END WHILE;
    COMMIT;
END $$
DELIMITER ;

GRANT EXECUTE ON PROCEDURE borrowBooks TO 'user'@'localhost';
GRANT EXECUTE ON PROCEDURE returnBooks TO 'user'@'localhost';



CREATE VIEW availableBook AS
SELECT Book.id, Book.title, Book.year, Author.firstName,Author.lastName,Category.name,Book.isbn
FROM Book
INNER JOIN Author ON Author.id=Book.authorId
LEFT JOIN Category_Book ON Book.id=Category_Book.bookId
LEFT JOIN Category ON Category_Book.categoryId=Category.id
WHERE Book.id NOT IN 
(
    SELECT DISTINCT Book.id
    FROM Book
    INNER JOIN Author ON Author.id = Book.authorId
    LEFT JOIN Borrow ON Book.id = Borrow.bookId
    LEFT JOIN Category_Book ON Book.id=Category_Book.bookId
    LEFT JOIN Category ON Category_Book.categoryId=Category.id
    WHERE Borrow.borrowDate IS NOT NULL AND Borrow.returnDate IS NULL
);


GRANT SELECT ON lista5.availableBook TO 'user'@'localhost';




CREATE VIEW ownBook AS
SELECT DISTINCT Book.id, Book.title, Book.year,User.nick
FROM Book
INNER JOIN Borrow ON Book.id=Borrow.bookId
INNER JOIN User ON User.id=Borrow.userId
WHERE Borrow.borrowDate IS NOT NULL AND Borrow.returnDate IS NULL;

GRANT SELECT ON lista5.ownBook TO 'user'@'localhost';


//////////////////////////////////////////////////////////



DELIMITER $$

CREATE PROCEDURE assignCategoryToBooks(
    IN bookTitle VARCHAR(255),
    IN categoryId INT
)
BEGIN
    DECLARE bookId INT;

    DECLARE done INT DEFAULT FALSE;

    DECLARE bookCursor CURSOR FOR
        SELECT id FROM Book
        WHERE title LIKE CONCAT('%', bookTitle, '%');

    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

    OPEN bookCursor;

    read_loop: LOOP
        FETCH bookCursor INTO bookId;

        IF done THEN
            LEAVE read_loop;
        END IF;

        INSERT INTO Category_Book (bookId, categoryId)
        VALUES (bookId, categoryId)
        ON DUPLICATE KEY UPDATE bookId = bookId;
    END LOOP;

    CLOSE bookCursor;
END $$

DELIMITER ;



//////////////////////////////////////////////////////////

DELIMITER $$

CREATE PROCEDURE assignCategory(
    IN bookTitle VARCHAR(255),
    IN categoryName VARCHAR(255)
)
BEGIN
    DECLARE number INT;
    DECLARE categoryId INT;

        
    SELECT COUNT(*) INTO number
    FROM Category
    WHERE Category.name LIKE categoryName;

    IF number=0 THEN
        INSERT INTO Category(name)
        VALUES(category);
    END IF;

    SELECT MIN(Category.id) INTO categoryId
    FROM Category
    WHERE Category.name LIKE categoryName;

    CALL assignCategoryToBooks(bookTitle,categoryId);
END $$

DELIMITER ;



CREATE VIEW salty AS
SELECT DISTINCT nick,salt
FROM User;

GRANT SELECT ON lista5.salty TO 'user'@'localhost';